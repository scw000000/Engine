#version 400

layout(location = 0) in vec2 iVertexPos;
layout(location = 1) in vec2 iUV;

out vec2 oUV;

void main()
    {
	gl_Position =  vec4( iVertexPos, -1.0, 1.0 );
	oUV = iUV;
    }

