#version 430
#define MAX_SAMPLE_LENGTH 100

uniform sampler2D   uLightingTex;
uniform sampler2D   uBlurTex;

in vec2 vUV;

out vec3 oColor;

void main()
    {
    oColor = texture( uLightingTex, vUV ).xyz + texture( uBlurTex, vUV ).xyz; 
    }

