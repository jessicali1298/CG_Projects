/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

#include <core/platform.h>
#include <core/integrator.h>

TR_NAMESPACE_BEGIN

/**
 * Surface normal integrator.
 */
struct NormalIntegrator : Integrator {
    explicit NormalIntegrator(const Scene& scene) : Integrator(scene) { }

    v3f render(const Ray& ray, Sampler& sampler) const override {
        v3f color(0.f, 0.f, 0.f);
	// TODO: Implement this
	/*
	 * If an intersection is found, simply return the
	 * component-wise absolute value of the shading normal
	 * at the intersection point, as a color.
	 */
	SurfaceInteraction info;
    bool intersect = scene.bvh->intersect(ray,info);
    if (intersect) {
        color = glm::abs(info.frameNs.n);
    }
    return color;
    }
};

TR_NAMESPACE_END