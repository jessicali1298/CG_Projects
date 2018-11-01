/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

#include "core/core.h"

TR_NAMESPACE_BEGIN

/**
 * Modified Phong reflectance model
 */
struct PhongBSDF : BSDF {

    std::unique_ptr<Texture < v3f>> specularReflectance;
    std::unique_ptr<Texture < v3f>> diffuseReflectance;
    std::unique_ptr<Texture < float>> exponent;
    float specularSamplingWeight;
    float scale;

    PhongBSDF(const WorldData& scene, const Config& config, const size_t& matID) : BSDF(scene, config, matID) {
        const tinyobj::material_t& mat = scene.materials[matID];

        if (mat.specular_texname.empty())
            specularReflectance = std::unique_ptr<Texture<v3f>>(new ConstantTexture3f(glm::make_vec3(mat.specular)));
        else
            specularReflectance = std::unique_ptr<Texture<v3f>>(new BitmapTexture3f(config, mat.specular_texname));

        if (mat.diffuse_texname.empty())
            diffuseReflectance = std::unique_ptr<Texture<v3f>>(new ConstantTexture3f(glm::make_vec3(mat.diffuse)));
        else
            diffuseReflectance = std::unique_ptr<Texture<v3f>>(new BitmapTexture3f(config, mat.diffuse_texname));

        exponent = std::unique_ptr<Texture<float>>(new ConstantTexture1f(mat.shininess));

        //get scale value to ensure energy conservation
        v3f maxValue = specularReflectance->getMax() + diffuseReflectance->getMax();
        float actualMax = max(max(maxValue.x, maxValue.y), maxValue.z);
        scale = actualMax > 1.0f ? 0.99f * (1.0f / actualMax) : 1.0f;

        float dAvg = getLuminance(diffuseReflectance->getAverage() * scale);
        float sAvg = getLuminance(specularReflectance->getAverage() * scale);
        specularSamplingWeight = sAvg / (dAvg + sAvg);

        components.push_back(EGlossyReflection);
        components.push_back(EDiffuseReflection);

        combinedType = 0;
        for (unsigned int component : components)
            combinedType |= component;
    }

    inline v3f reflect(const v3f& d) const {
        return v3f(-d.x, -d.y, d.z);
    }

    v3f eval(const SurfaceInteraction& i) const override {
        v3f val(0.f);
        // TODO: Add previous assignment code (if needed)
        //1. Check that the incoming ray is not hitting a backface (just like diffuse).
        //2. Evaluate the Phong BRDF using Equation (1).
        //3. Return this value multiplied by the cosine factor.

        v3f specDirection = reflect(i.wi);
        v3f diffuseReflect = diffuseReflectance->eval(worldData, i);
        v3f specReflect = specularReflectance->eval(worldData, i);
        float exp = exponent->eval(worldData, i);
        float cosAlpha = std::fmax(0.f,glm::dot(i.wo,specDirection)/glm::length(i.wo)/glm::length(specDirection));

        if (Frame::cosTheta(i.wi) >= 0 && Frame::cosTheta(i.wo) >= 0 ) {
            v3f phongBRDF = (diffuseReflect/M_PI) + (specReflect*((exp+2)/(2*M_PI))*glm::pow(cosAlpha, exp));
            //val = scale*(phongBRDF*Frame::cosTheta(i.wi));
            val = scale * phongBRDF;

        }
        return val;
    }

    float pdf(const SurfaceInteraction& i) const override {
        float pdf = 0.f;
        // TODO: Implement this
        float exp = exponent->eval(worldData, i);

        v3f viewReflect_world = glm::normalize(i.frameNs.toWorld(reflect(i.wo)));
        Frame phongFrame(viewReflect_world);

        v3f sample = phongFrame.toLocal(i.frameNs.toWorld(i.wi));
        pdf = Warp::squareToPhongLobePdf(sample,exp);

        return pdf;
    }

    v3f sample(SurfaceInteraction& i, const v2f& _sample, float* pdf) const override {
        v3f val(0.f);
        // TODO: Implement this
        v3f v, viewReflect_world;

        v3f sampleDir, sampleDir_world;
        float exp = exponent->eval(worldData, i);

        viewReflect_world = glm::normalize(i.frameNs.toWorld(reflect(i.wo)));
        Frame newFrame(viewReflect_world);

        //Transforming the direction from local coord. to world-space
        sampleDir = Warp::squareToPhongLobe(_sample, exp);

        v = Warp::squareToPhongLobe(_sample,exp);


        i.wi = glm::normalize(i.frameNs.toLocal(newFrame.toWorld(v)));
        *pdf = this->pdf(i);
        val = this->eval(i);
        return val;
    }

    std::string toString() const override { return "Phong"; }
};

TR_NAMESPACE_END