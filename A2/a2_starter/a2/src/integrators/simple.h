/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

TR_NAMESPACE_BEGIN

/**
 * Simple direct illumination integrator.
 */
struct SimpleIntegrator : Integrator {
    explicit SimpleIntegrator(const Scene& scene) : Integrator(scene) { }

    v3f render(const Ray& ray, Sampler& sampler) const override {
        v3f Li(0.f);
        // TODO: Implement this

        //PART 1.3
        //1. Retrieve the light position p and intensity I.
        //2. Intersect your ray with the scene geometry.
        //3. If an intersection i is found, retrieve the hit surface material using getBSDF(i).
        //4. Map the incoming direction i.wi to local coordinates by using the hit point frameNs.toLocal() transform.
        //5. Evaluate the BRDF locally and the light, and set the Li term accordingly.

        v3f position = scene.getFirstLightPosition();
        v3f intensity = scene.getFirstLightIntensity();
        SurfaceInteraction info;
        float R, R2;
        R = glm::length(info.p - position);
        R2 = glm::pow(R,2);
        bool intersect = scene.bvh->intersect(ray, info);
        info.wi = glm::normalize(info.frameNs.toLocal(position - info.p));
        if (intersect) {
            Li = glm::abs(intensity/(R2)*getBSDF(info)->eval(info));
        }

        //PART 1.4 Making Shadow
        v3f dir = glm::normalize(position - info.p);
        Ray shadowRay = Ray(info.p,dir);
        shadowRay.max_t = glm::length(position-info.p);
        if(scene.bvh->intersect(shadowRay, info)) {
                Li = v3f(0.f);
        }
        return Li;
    }
};

TR_NAMESPACE_END