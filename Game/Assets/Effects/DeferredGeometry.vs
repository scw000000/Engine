#version 400 

layout(location = 0) in vec3 iVertexPosition_Modelspace;
//layout(location = 1) in vec3 iVertexNormal_Modelspace;
//layout(location = 2) in vec2 iUV;

uniform mat4 uMVP;
//uniform mat4 uNMat;

//out vec3 vNormal;
//out vec2 vUV;
//out vec3 vPos;

void main(){
	gl_Position =  uMVP * vec4( iVertexPosition_Modelspace, 1 );
    //vPos = vec3( gl_Position.a / gl_Position.w, gl_Position.g / gl_Position.w, gl_Position.b / gl_Position.w );
    //gl_Position.z = 0;
    //vNormal = ( uNMat * vec4( iVertexNormal_Modelspace, 0 ) ).xyz;
    
    //vUV = iUV;
}

