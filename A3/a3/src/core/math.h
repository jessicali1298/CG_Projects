/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#pragma once

#include "core/core.h"
TR_NAMESPACE_BEGIN

/**
 * Computes barycentric coordinates.
 */
template<class T>
inline T barycentric(const T& a, const T& b, const T& c, const float u, const float v) {
    return a * (1 - u - v) + b * u + c * v;
}

/**
 * Restricts a value to a given interval.
 */
template<class T>
inline T clamp(T v, T min, T max) {
    return std::min(std::max(v, min), max);
}

/**
 * Checks if vector is zero.
 */
inline bool isZero(const v3f v) {
    return glm::dot(v, v) < Epsilon;
}

/**
 * Generates coordinate system.
 */
inline void coordinateSystem(const v3f& a, v3f& b, v3f& c) {
    if (std::abs(a.x) > std::abs(a.y)) {
        float invLen = 1.f / std::sqrt(a.x * a.x + a.z * a.z);
        c = v3f(a.z * invLen, 0.f, -a.x * invLen);
    } else {
        float invLen = 1.f / std::sqrt(a.y * a.y + a.z * a.z);
        c = v3f(0.f, a.z * invLen, -a.y * invLen);
    }
    b = glm::cross(c, a);
}

/**
 * Converts RGB value to luminance.
 */
inline float getLuminance(const v3f& rgb) {
    return glm::dot(rgb, v3f(0.212671f, 0.715160f, 0.072169f));
}

/**
 * Pseudo-random sampler (Mersenne Twister 19937) structure.
 */
struct Sampler {
    std::mt19937 g;
    std::uniform_real_distribution<float> d;
    explicit Sampler(int seed) {
        g = std::mt19937(seed);
        d = std::uniform_real_distribution<float>(0.f, 1.f);
    }
    float next() { return d(g); }
    p2f next2D() { return {d(g), d(g)}; }
    void setSeed(int seed) {
        g.seed(seed);
        d.reset();
    }
};

/**
 * 1D discrete distribution.
 */
struct Distribution1D {
    std::vector<float> cdf{0};
    bool isNormalized = false;

    inline void add(float pdfVal) {
        cdf.push_back(cdf.back() + pdfVal);
    }

    size_t size() {
        return cdf.size() - 1;
    }

    float normalize() {
        float sum = cdf.back();
        for (float& v : cdf) {
            v /= sum;
        }
        isNormalized = true;
        return sum;
    }

    inline float pdf(size_t i) const {
        assert(isNormalized);
        return cdf[i + 1] - cdf[i];
    }

    int sample(float sample) const {
        assert(isNormalized);
        const auto it = std::upper_bound(cdf.begin(), cdf.end(), sample);
        return clamp(int(distance(cdf.begin(), it)) - 1, 0, int(cdf.size()) - 2);
    }
};


/**
 * Warping functions.
 */
namespace Warp {


inline v3f squareToUniformSphere(const p2f& sample) {
    v3f v(0.f);
	// TODO: Implement this
	float r, phi, wx,wy,wz;
	wz = 2*sample.x - 1;
	r = glm::sqrt(1-glm::pow(wz,2));
	phi = 2*M_PI*sample.y;
	wx = r*glm::cos(phi);
	wy = r*glm::sin(phi);
	v = v3f(wx,wy,wz);

    return v;
}

inline float squareToUniformSpherePdf() {
    float pdf = 0.f;
	// TODO: Implement this
	pdf = 1/(4*M_PI);

    return pdf;
}

inline v3f squareToUniformHemisphere(const p2f& sample) {
    v3f v(0.f);
	// TODO: Implement this
    float r, phi, wx,wy,wz;
    wz = glm::abs(2*sample.x - 1);
    r = glm::sqrt(1-glm::pow(wz,2));
    phi = 2*M_PI*sample.y;
    wx = r*glm::cos(phi);
    wy = r*glm::sin(phi);
    v = v3f(wx,wy,wz);

    return v;
}

inline float squareToUniformHemispherePdf(const v3f& v) {
    float pdf = 0.f;
	// TODO: Implement this
	pdf = 1/(2*M_PI);
    return pdf;
}

inline v2f squareToUniformDiskConcentric(const p2f& sample) {
    v2f v(0.f);
	// TODO: Implement this (optional)
    float r, phi,wx,wy;
    r = sample.x;
    phi = 2*M_PI*sample.y;
    wx = sqrt(r)*glm::cos(phi);
    wy = sqrt(r)*glm::sin(phi);
    v = v2f(wx,wy);

    return v;
}

inline v3f squareToCosineHemisphere(const p2f& sample) {
    v3f v(0.f);
	// TODO: Implement this
	float wz;
    v2f DiskSample = squareToUniformDiskConcentric(sample);
    wz = glm::sqrt(1-pow(glm::length(DiskSample),2));
    v = v3f(DiskSample.x,DiskSample.y,wz);

    return v;
}

inline float squareToCosineHemispherePdf(const v3f& v) {
    float pdf = 0.f;
	// TODO: Implement this
	pdf = v.z/M_PI;

    return pdf;
}

inline v3f squareToPhongLobe(const p2f& sample) {
    v3f v(0.f);
	// TODO: Implement this
    float wx, wy, wz,theta, phi,exp;
    exp = 300.f;
    theta = glm::acos(glm::pow(1-sample.x,(1/(exp+2))));
    phi = 2*M_PI*sample.y;
    wx = glm::sin(theta)*glm::cos(phi);
    wy = glm::sin(theta)*glm::sin(phi);
    wz = cos(theta);
    v = v3f(wx,wy,wz);

    return v;
}

inline float squareToPhongLobePdf(const v3f& v) {
    float pdf = 0.f;
	// TODO: Implement this
	float exp = 300.f;
    pdf = (exp+2)/(2*M_PI)*glm::pow(v.z,exp);
    return pdf;
}

}

TR_NAMESPACE_END