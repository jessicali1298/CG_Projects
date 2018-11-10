/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

TR_NAMESPACE_BEGIN

/**
 * Direct illumination integrator with MIS
 */
struct DirectIntegrator : Integrator {
    explicit DirectIntegrator(const Scene& scene) : Integrator(scene) {
        m_emitterSamples = scene.config.integratorSettings.di.emitterSamples;
        m_bsdfSamples = scene.config.integratorSettings.di.bsdfSamples;
        m_samplingStrategy = scene.config.integratorSettings.di.samplingStrategy;
    }

    static inline float balanceHeuristic(float nf, float fPdf, float ng, float gPdf) {
        float f = nf * fPdf, g = ng * gPdf;
        return f / (f + g);
    }

    void sampleSphereByCosineHemisphere(const p2f& sample,
                                        const v3f& n,
                                        const p3f& pShading,
                                        const v3f& emitterCenter,
                                        float emitterRadius,
                                        v3f& wiW,
                                        float& pdf) const {
        // TODO: Implement this
    }

    void sampleSphereByArea(const p2f& sample,
                            const p3f& pShading,
                            const v3f& emitterCenter,
                            float emitterRadius,
                            v3f& pos,
                            v3f& ne,
                            v3f& wiW,
                            float& pdf) const {
        // TODO: Implement this
        //sampling a position y on sphere
        v3f sampleDir = Warp::squareToUniformSphere(sample);
        //evaluate PDF at that point
        pdf = 1/(4*M_PI*pow(emitterRadius,2.f));
        //set emitter(y) position
        pos = emitterCenter + emitterRadius * sampleDir;
        //set direction wi = x->y
        wiW = glm::normalize(pos - pShading);
        //set normal at y
        ne = glm::normalize(emitterRadius * sampleDir);

    }

    void sampleSphereBySolidAngle(const p2f& sample,
                                  const p3f& pShading,
                                  const v3f& emitterCenter,
                                  float emitterRadius,
                                  v3f& wiW,
                                  float& pdf) const {
        // TODO: Implement this
        float cosThetaMax = glm::sqrt(1.f-glm::pow((emitterRadius/glm::length(pShading-emitterCenter)),2.f));
        //float cosThetaMax = Frame::cosTheta((emitterCenter + emitterRadius)-pShading);

        v3f coneNormal = glm::normalize(emitterCenter - pShading);
        Frame coneFrame(coneNormal); //create local cone frame
        v3f sampleDir = Warp::squareToUniformCone(sample, cosThetaMax);

        wiW = glm::normalize(coneFrame.toWorld(sampleDir));
        pdf = Warp::squareToUniformConePdf(cosThetaMax);
    }

    v3f renderArea(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        SurfaceInteraction info,infoShadow;
        v3f sampleDir, sampleDir_world;
        v3f BRDF,pos,ne,wiW;
        float cosThetai,cosTheta0,jacobDet;
        float pdf;

        for (int i = 0; i < m_emitterSamples; i++) {
            v2f sample = v2f(sampler.next(),sampler.next());
            if(scene.bvh->intersect(ray,info)) {
                if (getEmission(info)!=v3f(0.f)) {
                    return getEmission(info);
                }
            }
            //select an emitter to sample
            float emPdf;
            size_t id = selectEmitter(sampler.next(), emPdf);
            const Emitter& em = getEmitterByID(id);

            //define emitter center and radius
            v3f emCenter = scene.getShapeCenter(em.shapeID);
            float emRadius = scene.getShapeRadius(em.shapeID);

            //pos, ne, wiW are all in World Space
            sampleSphereByArea(sample,info.p,emCenter,emRadius,pos,ne,wiW,pdf);

            info.wi = glm::normalize(info.frameNs.toLocal(wiW));

            cosThetai = Frame::cosTheta(info.wi);
            cosTheta0 = fmax(0.f,glm::dot(-wiW,ne)/glm::length(-wiW)/glm::length(ne));
            jacobDet = cosTheta0/glm::pow(glm::length(info.p-pos),2.f);

            Ray shadowRay = Ray(info.p, wiW, Epsilon);
            if (scene.bvh->intersect(shadowRay,infoShadow)) {
                if(getEmission(infoShadow) != v3f(0.f)) {
                    if (cosThetai >= 0.f) {
                        Lr += getEmission(infoShadow)*getBSDF(info)->eval(info)*cosThetai*jacobDet/pdf/emPdf;
                    }
                }
            }
        }
        Lr = Lr/m_emitterSamples;
        return Lr;
    }

    v3f renderCosineHemisphere(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        v3f v;
        SurfaceInteraction info,infoShadow;
        v3f sampleDir, sampleDir_world;
        float cosThetai;

        //shooting emitterSamples from shading point based on the input number of emitterSamples
        //The emitterSamples are distrubuted based on cosine-weighted hemispherical sampling
        for (int i = 0; i < m_emitterSamples; i++) {
            v2f sample = v2f(sampler.next(),sampler.next());

            //Cosine Hemisphere
            if (scene.bvh->intersect(ray,info)) {
                //If the eye-ray doesn't hit the light
                if (getEmission(info) != v3f(0.f)) {
                    return getEmission(info);
                }
                //Transforming the direction from local coord. to world-space
                sampleDir = Warp::squareToCosineHemisphere(sample);
                info.wi = sampleDir;
                sampleDir_world = glm::normalize(info.frameNs.toWorld(sampleDir));
                cosThetai = Frame::cosTheta(sampleDir);

                //shadowRay is now in world-space
                Ray shadowRay = Ray(info.p, sampleDir_world, Epsilon);
                if (scene.bvh->intersect(shadowRay,infoShadow)) {
                    if(getEmission(infoShadow) != v3f(0.f)) {
                        if (cosThetai >= 0.f) {
                            Lr += getEmission(infoShadow) * getBSDF(info)->eval(info) * cosThetai/ Warp::squareToCosineHemispherePdf(sampleDir);
                        }
                    }
                }
            }
        }
        Lr = Lr/m_emitterSamples;
        return Lr;
    }

    v3f renderBSDF(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        v3f v;
        SurfaceInteraction info,infoShadow;
        v3f sampleDir, sampleDir_world;
        v3f BRDF;
        float cosThetai;
        float pdf;

        //shooting emitterSamples from shading point based on the input number of emitterSamples
        //The emitterSamples are distrubuted based on cosine-weighted hemispherical sampling
        for (int i = 0; i < m_emitterSamples; i++) {
            v2f sample = v2f(sampler.next(),sampler.next());

            if (scene.bvh->intersect(ray,info)) {
                //If the eye-ray doesn't hit the light
                if (getEmission(info) != v3f(0.f)) {
                    return getEmission(info);
                }
                //Transforming the direction from local coord. to world-space
                BRDF = getBSDF(info)->sample(info,sample,&pdf); //set info.wi, calculate BRDF, and set PDF
                sampleDir_world = glm::normalize(info.frameNs.toWorld(info.wi));
                cosThetai = Frame::cosTheta(info.wi);

                //shadowRay is now in world-space
                Ray shadowRay = Ray(info.p, sampleDir_world, Epsilon);
                if (scene.bvh->intersect(shadowRay,infoShadow)) {
                    if(getEmission(infoShadow) != v3f(0.f)) {
                        if (cosThetai >= 0.f) {
                            Lr += getEmission(infoShadow) * BRDF * cosThetai/ pdf;
                        }
                    }
                }
            }
        }
        Lr = Lr/m_emitterSamples;
        return Lr;
    }

    v3f renderSolidAngle(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        SurfaceInteraction info,infoShadow;
        v3f sampleDir, sampleDir_world;
        v3f BRDF,pos,ne,wiW;
        float cosThetai,cosTheta0,jacobDet;
        float pdf;

        for (int i = 0; i < m_emitterSamples; i++) {
            v2f sample = v2f(sampler.next(),sampler.next());
            if(scene.bvh->intersect(ray,info)) {
                if (getEmission(info)!=v3f(0.f)) {
                    return getEmission(info);
                }
            }
            //select an emitter to sample
            float emPdf;
            size_t id = selectEmitter(sampler.next(), emPdf);
            const Emitter& em = getEmitterByID(id);

            //define emitter center and radius
            v3f emCenter = scene.getShapeCenter(em.shapeID);
            float emRadius = scene.getShapeRadius(em.shapeID);

            //sampleSphereByArea(sample,info.p,emCenter,emRadius,pos,ne,wiW,pdf);
            sampleSphereBySolidAngle(sample,info.p,emCenter,emRadius,wiW,pdf); //sets wiW, pdf

            info.wi = glm::normalize(info.frameNs.toLocal(wiW));

            cosThetai = Frame::cosTheta(info.wi);
            //cosTheta0 = fmax(0.f,glm::dot(-wiW,ne)/glm::length(-wiW)/glm::length(ne));
            //jacobDet = cosTheta0/glm::pow(glm::length(info.p-pos),2.f);

            Ray shadowRay = Ray(info.p, wiW, Epsilon);
            if (scene.bvh->intersect(shadowRay,infoShadow)) {
                if(getEmission(infoShadow) != v3f(0.f)) {
                    if (cosThetai >= 0.f) {
                        Lr += getEmission(infoShadow)*getBSDF(info)->eval(info)*cosThetai/pdf/emPdf;
                    }
                }
            }
        }
        Lr = Lr/m_emitterSamples;
        return Lr;
    }

    v3f renderMIS(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        v3f LrSA(0.f);
        v3f LrBRDF(0.f);
        // TODO: Implement this
        SurfaceInteraction info,infoShadow;
        v3f sampleDir, sampleDir_world;
        v3f BRDF,pos,ne,wiW;
        float cosThetai,cosTheta0,jacobDet;
        float pdfSA,pdfBRDF;

        //emitter sampling
        for (int i = 0; i < m_emitterSamples; i++) {
            v2f sample = v2f(sampler.next(),sampler.next());
            if(scene.bvh->intersect(ray,info)) {
                if (getEmission(info)!=v3f(0.f)) {
                    return getEmission(info);
                }
            }
            //select an emitter to sample
            float emPdf;
            size_t id = selectEmitter(sampler.next(), emPdf);
            const Emitter& em = getEmitterByID(id);

            //define emitter center and radius
            v3f emCenter = scene.getShapeCenter(em.shapeID);
            float emRadius = scene.getShapeRadius(em.shapeID);

            sampleSphereBySolidAngle(sample,info.p,emCenter,emRadius,wiW,pdfSA); //sets wiW, pdf

            info.wi = glm::normalize(info.frameNs.toLocal(wiW)); //set info.wi (shadingPoint's coords)
            cosThetai = Frame::cosTheta(info.wi);

            Ray shadowRay = Ray(info.p, wiW, Epsilon);
            if (scene.bvh->intersect(shadowRay,infoShadow)) {
                if(getEmission(infoShadow) != v3f(0.f)) {
                    if (cosThetai >= 0.f) {
                        pdfBRDF = getBSDF(info)->pdf(info);
                        float Wsa = balanceHeuristic(m_emitterSamples,pdfSA*emPdf,m_bsdfSamples,pdfBRDF);
                        LrSA += Wsa*getEmission(infoShadow)*getBSDF(info)->eval(info)*cosThetai/pdfSA/emPdf;
                    }
                }
            }
        }

        //BSDF sampling
        for (int j = 0; j < m_bsdfSamples; j++) {
            v2f sampleBSDF = v2f(sampler.next(),sampler.next());

            //Cosine Hemisphere
            if (scene.bvh->intersect(ray,info)) {
                //If the eye-ray doesn't hit the light
                if (getEmission(info) != v3f(0.f)) {
                    return getEmission(info);
                }
                //Transforming the direction from local coord. to world-space
                BRDF = getBSDF(info)->sample(info,sampleBSDF,&pdfBRDF); //set info.wi, calculate BRDF, and set PDF
                sampleDir_world = glm::normalize(info.frameNs.toWorld(info.wi));
                cosThetai = Frame::cosTheta(info.wi);

                //shadowRay is now in world-space
                Ray shadowRay = Ray(info.p, sampleDir_world, Epsilon);
                if (scene.bvh->intersect(shadowRay,infoShadow)) {
                    if(getEmission(infoShadow) != v3f(0.f)) {
                        if (cosThetai >= 0.f) {

                            //obtain the light that the shadowRay intersects with
                            const Emitter& em = getEmitterByID(getEmitterIDByShapeID(infoShadow.shapeID));
                            float emPDF = Integrator::getEmitterPdf(em);

                            //define emitter center and radius
                            v3f emCenter = scene.getShapeCenter(em.shapeID);
                            float emRadius = scene.getShapeRadius(em.shapeID);

                            //calculate pdfSA for Weighted function calculation
                            float cosThetaMax = glm::sqrt(1.f-glm::pow((emRadius/glm::length(info.p-emCenter)),2.f));
                            pdfSA = Warp::squareToUniformConePdf(cosThetaMax);

                            //Calculate weighted function
                            float WBr = balanceHeuristic(m_bsdfSamples,pdfBRDF,m_emitterSamples,pdfSA*emPDF);
                            LrBRDF += WBr*getEmission(infoShadow) * BRDF * cosThetai/ pdfBRDF;
                        }
                    }
                }
            }
        }
        if (m_emitterSamples == 0) {
            return LrBRDF/m_bsdfSamples;
        }
        if (m_bsdfSamples == 0) {
            return LrSA/m_emitterSamples;
        }
        LrSA = LrSA/m_emitterSamples;
        LrBRDF = LrBRDF/m_bsdfSamples;
        return LrSA + LrBRDF;
    }

    v3f render(const Ray& ray, Sampler& sampler) const override {
        if (m_samplingStrategy == "mis")
            return this->renderMIS(ray, sampler);
        else if (m_samplingStrategy == "area")
            return this->renderArea(ray, sampler);
        else if (m_samplingStrategy == "solidAngle")
            return this->renderSolidAngle(ray, sampler);
        else if (m_samplingStrategy == "cosineHemisphere")
            return this->renderCosineHemisphere(ray, sampler);
        else if (m_samplingStrategy == "bsdf")
            return this->renderBSDF(ray, sampler);
        std::cout << "Error: wrong strategy" << std::endl;
        exit(EXIT_FAILURE);
    }

    size_t m_emitterSamples;     // Number of emitter samples
    size_t m_bsdfSamples;        // Number of BSDF samples
    string m_samplingStrategy;   // Sampling strategy to use
};

TR_NAMESPACE_END