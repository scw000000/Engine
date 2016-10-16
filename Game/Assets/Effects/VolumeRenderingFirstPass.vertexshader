// for raycasting
#version 400

layout(location = 0) in vec3 iVerPos;
layout(location = 1) in vec3 iVerClr;

out vec3 oColor;

uniform mat4 uMVP;


void main()
{
    oColor = iVerClr;
    gl_Position = uMVP * vec4( iVerPos, 1.0 );
}
