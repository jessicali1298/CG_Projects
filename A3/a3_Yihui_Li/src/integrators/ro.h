/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once
#include <random>

TR_NAMESPACE_BEGIN

/**
 * Reflection occlusion integrator
 */

struct ROIntegrator : Integrator {

    float m_exponent;

    explicit ROIntegrator(const Scene& scene) : Integrator(scene) {
        m_exponent = scene.config.integratorSettings.ro.exponent;
    }

    inline v3f reflect(const v3f& d) const {
        return v3f(-d.x, -d.y, d.z);
    }

    v3f render(const Ray& ray, Sampler& sampler) const override {
        // TODO: Implement this
        v3f Li(0.f);
        v3f v,viewReflect_world, viewReflect_phong;
        SurfaceInteraction info;
        v2f sample = v2f(sampler.next(),sampler.next());
        v3f sampleDir, sampleDir_world;
        float cosThetai,BRDF,cosAlpha;


        if (scene.bvh->intersect(ray, info)) {

            viewReflect_world = glm::normalize(info.frameNs.toWorld(ROIntegrator::reflect(info.wo)));
            Frame newFrame(viewReflect_world);

            //Transforming the direction from local coord. to world-space
            sampleDir = Warp::squareToPhongLobe(sample);

            sampleDir_world = glm::normalize(newFrame.toWorld(sampleDir));

            //needs to be in the local frame of the shading point
            cosThetai = Frame::cosTheta(info.frameNs.toLocal(sampleDir_world));

            //computes and clamping cosAlpha
            viewReflect_phong = newFrame.toLocal(viewReflect_world);
            cosAlpha = glm::fmax(0.f,glm::dot(sampleDir,viewReflect_phong)/glm::length(sampleDir)/glm::length(viewReflect_phong));

            BRDF = ((m_exponent+2)/(2*M_PI))*fmax(0.f,glm::pow(cosAlpha,m_exponent));

            if (cosThetai >= 0.f && Frame::cosTheta(info.wo) >= 0.f  ) {
                Li = v3f(BRDF * cosThetai/ Warp::squareToPhongLobePdf(sampleDir));
            }
        }
        //shadowRay is now in world-space
        Ray shadowRay = Ray(info.p,sampleDir_world,Epsilon);
        if (scene.bvh->intersect(shadowRay, info)) {
            Li = v3f(0.f);
        }

        return Li;
    }
};

TR_NAMESPACE_END