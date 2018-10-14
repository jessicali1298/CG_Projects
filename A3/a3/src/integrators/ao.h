/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once


TR_NAMESPACE_BEGIN

/**
 * Ambient occlusion integrator
 */
struct AOIntegrator : Integrator {
    explicit AOIntegrator(const Scene& scene) : Integrator(scene) { }

//    1.Intersect your eye rays with the scene geometry.
//
//    2.If there's an intersection i, solve for the appropriate Monte Carlo
//      AO estimate at this shade point: you can sample directions in your
//      MC estimator using the sampling routines you developed earlier.
//
//    3.When evaluating the visibility in the AO integrand of your MC estimator,
//      take care when specifying relevant positions and directions;
//      remember, all intersection routines expect world-space coordinates.
//      Here, you will need to compute the parameters of a shadow ray based on i.p and i.wi.
//
//    4.When computing the contribution of each MC sample to the final integral estimate,
//      don't forget to evaluate all the integrand terms and to divide by the appropriate PDF evaluation.



    v3f render(const Ray& ray, Sampler& sampler) const override {
        // TODO: Implement this
        v3f Li(0.f);
        v3f v;
        SurfaceInteraction info;
        v2f sample = v2f(sampler.next(),sampler.next());
        v3f sampleDir, sampleDir_world;
        float cosThetai;

        //spherical sampling
//        if (scene.bvh->intersect(ray, info)) {
//            //Transforming the direction from local coord. to world-space
//            sampleDir = Warp::squareToUniformSphere(sample);
//            sampleDir_world = glm::normalize(info.frameNs.toWorld(sampleDir));
//            cosThetai = Frame::cosTheta(sampleDir);
//
//            if (cosThetai >= 0.f) {
//                Li = v3f(1.f / M_PI * cosThetai/ Warp::squareToUniformSpherePdf());
//            }
//        }
//        //shadowRay is now in world-space
//        Ray shadowRay = Ray(info.p,sampleDir_world, Epsilon, scene.aabb.getBSphere().radius/2);
//        if (scene.bvh->intersect(shadowRay, info)) {
//            Li = v3f(0.f);
//        }


        //Hemispherical sampling
//        if (scene.bvh->intersect(ray, info)) {
//            //Transforming the direction from local coord. to world-space
//            sampleDir = Warp::squareToUniformHemisphere(sample);
//            sampleDir_world = glm::normalize(info.frameNs.toWorld(sampleDir));
//            cosThetai = Frame::cosTheta(sampleDir);
//
//            if (cosThetai >= 0.f) {
//                Li = v3f(1.f / M_PI * cosThetai/ Warp::squareToUniformHemispherePdf(sampleDir));
//            }
//        }
//        //shadowRay is now in world-space
//        Ray shadowRay = Ray(info.p,sampleDir_world, Epsilon, scene.aabb.getBSphere().radius/2);
//        if (scene.bvh->intersect(shadowRay, info)) {
//            Li = v3f(0.f);
//        }


        //Cosine Hemisphere
        if (scene.bvh->intersect(ray, info)) {
            //Transforming the direction from local coord. to world-space
            sampleDir = Warp::squareToCosineHemisphere(sample);
            sampleDir_world = glm::normalize(info.frameNs.toWorld(sampleDir));
            cosThetai = Frame::cosTheta(sampleDir);

            if (cosThetai >= 0.f) {
                Li = v3f(1.f / M_PI * cosThetai/ Warp::squareToCosineHemispherePdf(sampleDir));
            }
        }
        //shadowRay is now in world-space
        Ray shadowRay = Ray(info.p,sampleDir_world, Epsilon, scene.aabb.getBSphere().radius/2);
        if (scene.bvh->intersect(shadowRay, info)) {
            Li = v3f(0.f);
        }



        return Li;
    }
};

TR_NAMESPACE_END