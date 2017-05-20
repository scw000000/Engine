#version 330 core

layout(location = 0) in vec3 iVertexPosition_Modelspace;
layout(location = 1) in vec2 iVertexUV;
layout(location = 2) in vec3 iVertexNormal_Modelspace;
layout(location = 3) in uvec4 iBoneIDs;
layout(location = 4) in vec4 iBoneWeights;

out vec2 oUV;
out vec3 oPosition_WorldSpace;
out vec3 oNormal_WorldSpace;
out vec3 oEyeDirection_WorldSpace;

uniform mat4 	uMVP;
uniform mat4 	uM;

uniform vec3	uEyePosition_WorldSpace;

const int MAXIMUM_BONES_PER_ACTOR = 100;

uniform mat4    uBoneTransform[ MAXIMUM_BONES_PER_ACTOR ];

void main()
	{
	mat4 boneMat = uBoneTransform[ iBoneIDs[ 0 ] ] * iBoneWeights[ 0 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 1 ] ] * iBoneWeights[ 1 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 2 ] ] * iBoneWeights[ 2 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 3 ] ] * iBoneWeights[ 3 ];
	
	vec4 VertexAniPosition_ModelSpace = boneMat * vec4( iVertexPosition_Modelspace, 1 );
    
	gl_Position =  uMVP * VertexAniPosition_ModelSpace;
	
	oPosition_WorldSpace = ( uM * VertexAniPosition_ModelSpace ).xyz;
	
	oEyeDirection_WorldSpace = uEyePosition_WorldSpace - oPosition_WorldSpace;
	
	oNormal_WorldSpace = ( uM * vec4( iVertexNormal_Modelspace, 0 ) ) .xyz; 
    
	oUV = iVertexUV;
	}

