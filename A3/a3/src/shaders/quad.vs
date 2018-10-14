/*
    This file is part of TinyRender, an educative PBR system.
    Designed for ECSE 446/546 Realistic Image Synthesis, McGill University.

    Copyright (c) 2018 by Derek Nowrouzezahrai and others.
*/
#version 330 core

#This input position is in world-space
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

out vec2 texCoords;
out vec2 gl_Position;

void main() {
    texCoords = uv;
    gl_Position = vec4(position,0.0,1.0);
}
