#version 400 

layout(location = 0) in vec3 iVertexPosMS;
layout(location = 1) in vec3 iVertexNormalMS;
layout(location = 2) in vec2 iUV;
// layout(location = 2) in vec2 iVertexTangentMS;
// layout(location = 2) in vec2 iVertexBitangentMS;

uniform mat4 uMVP;
uniform mat4 uNormal;

out vec3 vNormalVS;
out vec2 vUV;
//out vec3 vPos;

void main(){
	gl_Position =  uMVP * vec4( iVertexPosMS, 1 );
    //vPos = vec3( gl_Position.a / gl_Position.w, gl_Position.g / gl_Position.w, gl_Position.b / gl_Position.w );
    //gl_Position.z = 0;
    vNormalVS = normalize( ( uNormal * vec4( iVertexNormalMS, 0 ) ).xyz );
    
    vUV = iUV;
}

