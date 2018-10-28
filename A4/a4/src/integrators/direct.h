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
    }

    void sampleSphereBySolidAngle(const p2f& sample,
                                  const p3f& pShading,
                                  const v3f& emitterCenter,
                                  float emitterRadius,
                                  v3f& wiW,
                                  float& pdf) const {
        // TODO: Implement this
    }

    v3f renderArea(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        return Lr;
    }

    v3f renderCosineHemisphere(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        v3f tempLr(0.f);
        // TODO: Implement this
        v3f v;
        SurfaceInteraction info,infoShadow;
        //v2f sample = v2f(sampler.next(),sampler.next());
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
        return Lr;
    }

    v3f renderSolidAngle(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        return Lr;
    }

    v3f renderMIS(const Ray& ray, Sampler& sampler) const {
        v3f Lr(0.f);
        // TODO: Implement this
        return Lr;
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