#version 400
layout(location = 0) in vec3 iVertexPosition_Modelspace;

uniform mat4 	uMVP;

void main()
	{
	gl_Position =  uMVP * vec4( iVertexPosition_Modelspace, 1 );
	}

