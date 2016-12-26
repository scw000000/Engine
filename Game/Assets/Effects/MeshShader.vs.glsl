#version 400
const int MAXIMUM_LIGHTS_SUPPORTED = 8;

layout(location = 0) in vec3 iVertexPosition_Modelspace;
layout(location = 1) in vec2 iVertexUV;
layout(location = 2) in vec3 iVertexNormal_Modelspace;

out vec2 vUV;
out vec3 vPosition_WorldSpace;
out vec3 vNormal_WorldSpace;
out vec3 vEyeDirection_WorldSpace;
//out vec4 vShadowMapTexCoord[ MAXIMUM_LIGHTS_SUPPORTED ];

uniform mat4 	uMVP;
uniform mat4 	uM;

uniform vec3	uEyePosition_WorldSpace;

//uniform int     uLightNumber;
//uniform mat4    uShadowMapMatrix[ MAXIMUM_LIGHTS_SUPPORTED ];

void main()
	{
	gl_Position =  uMVP * vec4( iVertexPosition_Modelspace, 1 );
	
    vec4 vertexPosition_WorldSpace = uM * vec4( iVertexPosition_Modelspace, 1 );
	vPosition_WorldSpace = vertexPosition_WorldSpace.xyz;
	
	vEyeDirection_WorldSpace = normalize( uEyePosition_WorldSpace - vPosition_WorldSpace );
	
	// Normal of the the vertex, in camera space
    // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not
	vNormal_WorldSpace = normalize( ( uM * vec4( iVertexNormal_Modelspace, 0 ) ) .xyz );
	
	vUV = iVertexUV;
    
    //for( int i = 0; i < uLightNumber; ++i )
    //    {
    //    vShadowMapTexCoord[ i ] = ( uShadowMapMatrix[ i ] * vertexPosition_WorldSpace );
    //    }
	}

