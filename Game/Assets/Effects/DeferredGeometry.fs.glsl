#version 400

in vec3 vNormalVS;
in vec2 vUV;

uniform sampler2D   uAlbedoTex;

layout(location = 0) out vec4 oMRT0; // normal + glossiness
layout(location = 1) out vec4 oMRT1; // albedo + metalness

void main(){
    oMRT0.xyz = vNormalVS;
    oMRT0.w = 0.3;
    
    oMRT0.xyz = texture( uAlbedoTex, vUV ).rgb;
    oMRT0.w = 0.3;
}