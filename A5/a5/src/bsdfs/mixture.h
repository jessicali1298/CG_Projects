/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

#include "core/core.h"

TR_NAMESPACE_BEGIN

/**
 * Modified Phong reflectance model + Diffuse
 */
struct MixtureBSDF : BSDF {
    std::unique_ptr<Texture < v3f>> specularReflectance;
    std::unique_ptr<Texture < v3f>> diffuseReflectance;
    std::unique_ptr<Texture < float>> exponent;
    float specularSamplingWeight;
    float scale;

    MixtureBSDF(const WorldData& scene, const Config& config, const size_t& matID) : BSDF(scene, config, matID) {
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
            val = scale * phongBRDF;

        }
        return val;
    }

    float pdf(const SurfaceInteraction& i) const override {
        float pdf = 0.f;
        // TODO: Implement this
        float pdfSpec,pdfDiff;
        float exp = exponent->eval(worldData, i);

        v3f viewReflect_world = glm::normalize(i.frameNs.toWorld(reflect(i.wo)));
        Frame phongFrame(viewReflect_world);

        v3f sample = phongFrame.toLocal(i.frameNs.toWorld(i.wi));
        pdfSpec = Warp::squareToPhongLobePdf(sample,exp);

        pdfDiff = Warp::squareToCosineHemispherePdf(i.wi);
        pdf = specularSamplingWeight*pdfSpec + (1-specularSamplingWeight)*pdfDiff;

        return pdf;
    }

    v3f sample(SurfaceInteraction& i, const v2f& _sample, float* pdf) const override {
        v3f val(0.f);
        // TODO: Implement this
        // 1. determine the reflectance coeff for both diffuse and specular
        // 2. Use these coeff as guidelines for distribution of each type of samples
        // 3. Do Russian roulette on e.g. the x-component of the _sample to determine the scattering type
        //    Then you need to rescale this component according to the type

        // 4. Perform Cosine-Weighted/Phong-Lobe accordingly to obtain sample
        // 5. Use this sample to calculate corresponding BRDF (Diffuse or Specular)
        //    by calling MixtureBRDF:eval()

        v2f sampleCopy = _sample;


        float RRprob = sampleCopy.x;


        //if RRprob leads to diffuse sampling
        if (RRprob <= specularSamplingWeight) {
            sampleCopy.x = sampleCopy.x/specularSamplingWeight;

            v3f viewReflect_world;
            v3f sampleDir;
            float exp = exponent->eval(worldData, i);
            viewReflect_world = glm::normalize(i.frameNs.toWorld(reflect(i.wo)));
            Frame newFrame(viewReflect_world);

            sampleDir = Warp::squareToPhongLobe(sampleCopy, exp);

            //i.wi now in local space of the shading point
            i.wi = glm::normalize(i.frameNs.toLocal(newFrame.toWorld(sampleDir)));

        }

        else {
            sampleCopy.x = sampleCopy.x/(1-specularSamplingWeight);
            i.wi = Warp::squareToCosineHemisphere(sampleCopy);
        }

        *pdf = this->pdf(i);
        val = this->eval(i);
        return val;
    }

    std::string toString() const override { return "Mixture"; }
};

TR_NAMESPACE_END