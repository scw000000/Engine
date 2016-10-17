#version 400


layout (location = 0) in vec3 iVerPos;
layout (location = 1) in vec3 iVerClr;  

out vec3 oEntryPoint_ModelSpace;

uniform mat4 uMVP;

void main()
{
    oEntryPoint_ModelSpace = iVerClr;
    gl_Position = uMVP * vec4( iVerPos, 1.0 );
    // ExitPointCoord 输入到fragment shader 的过程中经过rasterization， interpolation, assembly primitive
}
