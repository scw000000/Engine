#version 400

layout(location = 0) in vec3 iVertexPosMS;
layout(location = 1) in vec3 iVertexNormalMS;
layout(location = 2) in vec2 iUV;
layout(location = 3) in vec3 iVertexTangentMS;
layout(location = 4) in vec3 iVertexBitangentMS;
layout(location = 5) in uvec4 iBoneIDs;
layout(location = 6) in vec4 iBoneWeights;

/////
out vec3 vTangentVS;
out vec3 vBitangentVS;
out vec3 vNormalVS;
out vec4 vCurrPosCS;
out vec4 vPrevPosCS;

out vec2 vUV;

uniform mat4 uMVP;
uniform mat4 uPrevMVP;

uniform mat4 uNormal;

const int MAXIMUM_BONES_PER_ACTOR = 200;

uniform mat4    uBoneTransform[ MAXIMUM_BONES_PER_ACTOR ];

void main()
	{
	mat4 boneMat = uBoneTransform[ iBoneIDs[ 0 ] ] * iBoneWeights[ 0 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 1 ] ] * iBoneWeights[ 1 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 2 ] ] * iBoneWeights[ 2 ];
    boneMat     += uBoneTransform[ iBoneIDs[ 3 ] ] * iBoneWeights[ 3 ];
    
    vec4 vertexAniPosition_ModelSpace = boneMat * vec4( iVertexPosMS, 1 );
    
	gl_Position =  uMVP * vertexAniPosition_ModelSpace;
	// gl_Position =  uMVP * vec4( iVertexPosMS, 1 );
    vCurrPosCS = gl_Position;
    vPrevPosCS = uPrevMVP * vec4( iVertexPosMS, 1 );
    
	vNormalVS = normalize( ( uNormal * vec4( iVertexNormalMS, 0 ) ).xyz );
    vTangentVS = normalize( ( uNormal * vec4( iVertexTangentMS, 0 ) ).xyz );
    vBitangentVS = normalize( ( uNormal * vec4( iVertexBitangentMS, 0 ) ).xyz );
    
	vUV = iUV;
	}

