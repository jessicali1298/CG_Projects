/*
    This file is part of TinyRender, an educative rendering system.

    Designed for ECSE 446/546 Realistic/Advanced Image Synthesis.
    Derek Nowrouzezahrai, McGill University.
*/

#version 330 core

#define M_PI       3.14159265358979323846f
uniform vec3 camPos;
uniform vec3 lightPos;
uniform vec3 lightIntensity;
uniform vec3 rho_d;
uniform vec3 rho_s;
uniform float exponent;

in vec3 vPos;
in vec3 vNormal;
out vec3 phongColor;

void main() {
    phongColor = vec3(0.f);
    vec3 wi = lightPos - vPos;
    vec3 wo = camPos - vPos;

    vec3 specDirectionRT = 2 * normalize(vNormal) * (dot(vNormal,wi)/length(vNormal)/length(wi)) - normalize(wi);
    float cosAlphaRT = abs(dot(wo,specDirectionRT)/length(wo)/length(specDirectionRT));
    float wi_bound = dot(vNormal,wi)/length(vNormal)/length(wi);
    float wo_bound = dot(vNormal,wo)/length(vNormal)/length(wo);

    if(wi_bound>=0.f && wo_bound>=0.f) {
        float cosFactor = dot(vNormal,wi)/length(vNormal)/length(wi);
        vec3 phongBRDF_RT = ((rho_d/M_PI) + (rho_s*((exponent+2)/(2*M_PI))*pow(cosAlphaRT,exponent)))*cosFactor;
        float R2_RT = pow(length(vPos - lightPos),2);
        phongColor = abs(lightIntensity/(R2_RT)*phongBRDF_RT);
    }


}