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


        SurfaceInteraction info;
        float R2;

        if (scene.bvh->intersect(ray, info)) {
                v3f position = scene.getFirstLightPosition();
                v3f intensity = scene.getFirstLightIntensity();
                R2 = glm::pow(glm::length(info.p - position),2);
                info.wi = glm::normalize(info.frameNs.toLocal(position - info.p));
                Li = intensity/(R2)*getBSDF(info)->eval(info);

                //PART 1.4 Making Shadow
                v3f sDir = glm::normalize(position - info.p);
                Ray shadowRay = TinyRender::Ray(info.p,sDir,Epsilon,glm::length(position-info.p) - Epsilon);
                if(scene.bvh->intersect(shadowRay)) {
                        Li = v3f(0.f);
                }
        }

        //PART 1.4 Making Shadow
//        v3f dir = glm::normalize(position - info.p);
//        Ray shadowRay = Ray(info.p,dir,Epsion, glm::length(position-info.p) - Epsilon);
//        if(scene.bvh->intersect(shadowRay, info)) {
//                Li = v3f(0.f);
//        }
        return glm::abs(Li);
    }
};

TR_NAMESPACE_END