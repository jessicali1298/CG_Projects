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
uniform vec3 albedo;

in vec3 vPos;
in vec3 vNormal;
out vec3 diffuseColor;

void main() {
    diffuseColor = vec3(0.f);
    vec3 wi = lightPos - vPos;
    vec3 wo = camPos - vPos;
    float wi_bound = dot(vNormal,wi)/length(vNormal)/length(wi);
    float wo_bound = dot(vNormal,wo)/length(vNormal)/length(wo);

    if(wi_bound>=0.f && wo_bound>=0.f) {
        float cosFactor = dot(vNormal,wi)/length(vNormal)/length(wi);
        vec3 diffuseBRDF_RT = albedo/M_PI*cosFactor;
        float R2_RT = pow(length(vPos - lightPos),2);
        diffuseColor = abs(lightIntensity/(R2_RT)*diffuseBRDF_RT);
    }

}