/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

TR_NAMESPACE_BEGIN

/**
 * Path tracer integrator
 */
    struct PathTracerIntegrator : Integrator {
        explicit PathTracerIntegrator(const Scene& scene) : Integrator(scene) {
            m_isExplicit = scene.config.integratorSettings.pt.isExplicit;
            m_maxDepth = scene.config.integratorSettings.pt.maxDepth;
            m_rrDepth = scene.config.integratorSettings.pt.rrDepth;
            m_rrProb = scene.config.integratorSettings.pt.rrProb;
        }



        v3f renderImplicit(const Ray& ray, Sampler& sampler, SurfaceInteraction& hit, int bounce) const {
            v3f Li(0.f);

            // TODO: Implement this
            v3f v;
            SurfaceInteraction info,infoShadow;
            v3f sampleDir, sampleDir_world;
            v3f BRDF;
            float cosThetai;
            float pdf;

            v2f sample = v2f(sampler.next(),sampler.next());

            //if eye ray directly sees the light, return light's emission
            float backcheck = glm::dot(hit.frameNs.n, -ray.d);
            if (getEmission(hit) != v3f(0.f)) {
                if (backcheck >0) {
                    return getEmission(hit);
                }
                else {
                    return v3f(0.f);
                }
            }
            //if eye ray doesn't see the light, create a shadowRay and bounce until light is seen
            //or maxDepth has reached
            BRDF = getBSDF(hit)->sample(hit,sample,&pdf); //set info.wi, calculate BRDF, and set PDF
            sampleDir_world = glm::normalize(hit.frameNs.toWorld(hit.wi));
            cosThetai = Frame::cosTheta(hit.wi);

            Ray shadowRay = Ray(hit.p, sampleDir_world, Epsilon);

            if (scene.bvh->intersect(shadowRay,infoShadow) && (bounce <= m_maxDepth-1)) {
                if (pdf<=0.f) {
                    return v3f(0.f);
                }
                if (cosThetai >= 0.f) {
                    bounce++;
                    Li = BRDF * renderImplicit(shadowRay,sampler,infoShadow,bounce) * cosThetai / pdf;
                    return Li;
                }
            }
            return Li;
        }


        v3f renderExplicit(const Ray& ray, Sampler& sampler, SurfaceInteraction& hit, int recursion) const {
            v3f Li(0.f),Ld(0.f),Lind(0.f);
            // TODO: Implement this

            float rrFactor = 1.f;
            //check if can hit light directly
            v3f emission = getEmission(hit);
            if (emission != v3f(0.f)) {
                return emission;
            }
            if (recursion<=m_maxDepth-1 || m_maxDepth == -1) {
                recursion++;

                //if number of recursions exceed m_rrDepth (min bounces needed to start RR)
                if (recursion >= m_rrDepth) {
                    if (sampler.next() >= m_rrProb) {
//                        return v3f(1.f/m_rrProb);
                        return v3f(0.f);
                    }
                    else {
                        rrFactor = 1.f/m_rrProb;
                    }
                }
                Ld = DirectLight(hit,sampler);

                if(m_maxDepth > 1 || m_maxDepth == -1) {
                    /** indirect illumnination */
                    Lind = rrFactor*IndirectLight(ray, sampler, hit ,recursion);
                }
            }
            return Ld+Lind;
        }


        v3f IndirectLight(const Ray& ray, Sampler& sampler, SurfaceInteraction& hit, int recursion) const {
            v3f Lind(0.f);
            float pdf;
            SurfaceInteraction shadowInfo;
            v2f sample = v2f(sampler.next(),sampler.next());

            v3f BRDF = getBSDF(hit)->sample(hit, sample, &pdf);
            v3f wiW = glm::normalize(hit.frameNs.toWorld(hit.wi));
            float cosThetai = Frame::cosTheta(hit.wi);

            Ray shadowRay = TinyRender::Ray(hit.p, wiW, Epsilon);


            if (scene.bvh->intersect(shadowRay, shadowInfo)) {
                v3f emission = getEmission(shadowInfo);

                //sample another indirect ray if hit the light
                if (emission != v3f(0.f)) {
                    return v3f(0.f);
                }
                else{
                    /** did not hit a light, recursive call*/
                    if(pdf == 0){
                        return v3f(0.f);
                    }
                    Lind = renderExplicit(shadowRay,sampler,shadowInfo,recursion) * BRDF * glm::fmax(0.f, cosThetai)/pdf;
                    return Lind;
                }
            }
            else{
                /** shadowRay exits the scene and hits nothing*/
                return v3f(0.f);
            }
        }

        v3f DirectLight(SurfaceInteraction& info, Sampler& sampler) const {
            v3f Lr(0.f);
            // TODO: Implement this
            float emPDF, areaPDF;
            size_t id = selectEmitter(sampler.next(), emPDF);
            const Emitter& em = getEmitterByID(id);
            v3f normal,pos;
            SurfaceInteraction infoShadow;

            //sets pos, normal, pdf
            sampleEmitterPosition(sampler, em, normal, pos, areaPDF);

            v3f directContribution = v3f(0.f);
            v3f wiW = glm::normalize(pos - info.p);
            info.wi = glm::normalize(info.frameNs.toLocal(wiW));

            float cosThetai = Frame::cosTheta(info.wi);
            float cosTheta0 = fmax(0.f,glm::dot(-wiW,normal)/glm::length(-wiW)/glm::length(normal));
            float jacobDet = cosTheta0/glm::pow(glm::length(info.p-pos),2.f);

            Ray shadowRay = Ray(info.p, wiW, Epsilon);
            if (scene.bvh->intersect(shadowRay, infoShadow)) {
                //if hit the light, return the corresponding illumination info
                if (getEmission(infoShadow) != v3f(0.f)) {
                    if ((emPDF*areaPDF) <= 0.f) {
                        return v3f(0.f);
                    }

                    if (cosThetai >= 0.f) {
                        Lr = getEmission(infoShadow) * getBSDF(info)->eval(info)*jacobDet*cosThetai/emPDF/areaPDF;
                    }
                }
            }
            return Lr;
        }


        v3f render(const Ray& ray, Sampler& sampler) const override {
            Ray r = ray;
            SurfaceInteraction hit;

            if (scene.bvh->intersect(r, hit)) {
                if (m_isExplicit)
                    return this->renderExplicit(ray, sampler, hit,0);
                else
                    return this->renderImplicit(ray, sampler, hit,0);
            }
            return v3f(0.0);
        }

        int m_maxDepth;     // Maximum number of bounces
        int m_rrDepth;      // When to start Russian roulette
        float m_rrProb;     // Russian roulette probability
        bool m_isExplicit;  // Implicit or explicit
    };

TR_NAMESPACE_END
