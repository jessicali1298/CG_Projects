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
            v3f Li(0.f),Ld(0.f),Lind(0.f);

            // TODO: Implement this
            v3f v;
            SurfaceInteraction info,infoShadow;
            v3f sampleDir, sampleDir_world;
            v3f BRDF;
            float cosThetai;
            float pdf;

            v2f sample = v2f(sampler.next(),sampler.next());

//            if (scene.bvh->intersect(ray,hit)) {
                //if eye ray directly sees the light, return light's emission
//                float backcheck = glm::dot(v3f(0.f, 0.f, 1.f), hit.wo);
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
//                    cout << pdf;
                    Li = BRDF * renderImplicit(shadowRay,sampler,infoShadow,bounce) * cosThetai / pdf;
                    return Li;
                }
            }
//            else {
//                return v3f(0.f);
//            }
//
//            Li = Lind;
            return Li;
        }


        v3f renderExplicit(const Ray& ray, Sampler& sampler, SurfaceInteraction& hit) const {
            v3f Li(0.f);
            // TODO: Implement this
//            v3f emission = getEmission(hit);
//            if (emission != v3f(0.f)) {
//                return emission;
//            }
//
//            v3f Ld;
//            v3f Lind;
//            if (m_maxDepth == 0) {
//                Ld = v3f(0.f);
//            }
//            else {
//                Ld = DirectLight(hit, sampler);
//            }
//            int currentDepth = 0;
//            Lind = indirectLight(hit, sampler, currentDepth);
//            Li = Ld + Lind;

            return Li;
        }


//        v3f indirectLight(SurfaceInteraction& info, Sampler& sampler, int depth) const {
//            //add 1 to bounce to keep track of bounces
//            int recursiveDepth = depth+1;
//            float russianStop = 1.f;
//            v3f zeroContri(0.f);
//
//            //if depth has reached maxDepth, return 0
//            if (recursiveDepth >= m_maxDepth) {
//                return v3f(0.f);
//            }
//
//            v3f holdingProduct = v3f(1.f);
//            v3f emission = v3f(1.f);
//            SurfaceInteraction rayInfo;
//
//            //keep tracing if hits the light
//            while (emission != v3f(0.f)) {
//                float pdf;
//                holdingProduct = getBSDF(info)->sample(info, sampler.next2D(), &pdf);
//                v3f wiW = info.frameNs.toWorld(info.wi);
//                wiW = glm::normalize(wiW);
//                float cosThetai = Frame::cosTheta(info.wi);
//
//                Ray recursiveRay = Ray(info.p, wiW);
//
//                if (scene.bvh->intersect(recursiveRay, rayInfo)) {
//                    emission = getEmission(rayInfo);
//                }
//                else {
//                    return v3f(0.f);
//                }
//            }
//            return  1 / russianStop * holdingProduct*(DirectLight(rayInfo, sampler) + indirectLight(rayInfo, sampler, recursiveDepth))*cosThetai/pdf;
//        }

//        v3f DirectLight(SurfaceInteraction& info, Sampler& sampler) const {
//            v3f Lr(0.f);
//            // TODO: Implement this
//            float emPDF, areaPDF;
//            size_t id = selectEmitter(sampler.next(), emPDF);
//            const Emitter& em = getEmitterByID(id);
//            v3f normal,pos;
//            SurfaceInteraction infoShadow;
//
//            //sets pos, normal, pdf
//            sampleEmitterPosition(sampler, em, normal, pos, areaPDF);
//
//            v3f directContribution = v3f(0.f);
//            v3f wiW = glm::normalize(pos - info.p);
//            info.wi = glm::normalize(info.frameNs.toLocal(wiW));
//
//            float cosThetai = Frame::cosTheta(info.wi);
//            float cosTheta0 = fmax(0.f,glm::dot(-wiW,normal)/glm::length(-wiW)/glm::length(normal));
//            float jacobDet = cosTheta0/glm::pow(glm::length(info.p-pos),2.f);
//
//            Ray shadowRay = Ray(info.p, wiW, Epsilon);
//            if (scene.bvh->intersect(shadowRay, infoShadow)) {
//                if (getEmission(infoShadow) != v3f(0.f)) {
//                    if (cosThetai >= 0.f) {
//                        Lr = getEmission(infoShadow) * getBSDF(info)->eval(info)*jacobDet*cosThetai/emPDF/areaPDF;
//                    }
//                }
//            }
//
//            return Lr;
//        }


        v3f render(const Ray& ray, Sampler& sampler) const override {
            Ray r = ray;
            SurfaceInteraction hit;

            if (scene.bvh->intersect(r, hit)) {
                if (m_isExplicit)
                    return this->renderExplicit(ray, sampler, hit);
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
