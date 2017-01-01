#version 430

layout ( location = 0 ) in vec2 iVertexPosMS;
layout ( location = 1 ) in vec2 iUV;

out vec2 vUV

void main
    {
    vUV = iUV;
    }

