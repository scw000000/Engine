#version 400
const int MAXIMUM_LIGHTS_SUPPORTED = 8;

in vec2 vUV;
in vec3 vPosition_WorldSpace;
in vec3 vNormal_WorldSpace;
in vec3 vEyeDirection_WorldSpace;
//in vec4 vShadowMapTexCoord[ MAXIMUM_LIGHTS_SUPPORTED ];

uniform vec3 	    uLightPosition_WorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform vec3 	    uLighDirection_WorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform vec3	    uLightColor[ MAXIMUM_LIGHTS_SUPPORTED ];
//uniform float       uLightPower[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform int         uLightNumber;

//uniform sampler2D   uShadowMapTexture[ MAXIMUM_LIGHTS_SUPPORTED ];

// Values that stay constant for the whole mesh.
uniform sampler2D   uMeshTexture;
uniform vec4 	    uMaterialDiffuse;
uniform vec3	    uMaterialAmbient;
uniform vec3	    uMaterialSpecular;

// Ouput data
out vec4 color;

void main()
    {
     vec3 N = vNormal_WorldSpace;
     vec3 E = vEyeDirection_WorldSpace;
     
    vec3 textureColor = texture( uMeshTexture, vUV ).rgb;
    color = vec4( 0, 0, 0, 1 );
    
   for( int i = 0; i < uLightNumber; ++i )
         {
         //float currZ = vShadowMapTexCoord[ i ].z / vShadowMapTexCoord[ i ].w;
         //float shadowMapZ = texture( uShadowMapTexture[ i ], vShadowMapTexCoord[ i ].xy / vShadowMapTexCoord[ i ].w ).z;
         //float visibility = 1.0;
         //if( shadowMapZ < currZ )
         //   {
         //   visibility = 0.5;
         //   }
         float distanceSqr = length( uLightPosition_WorldSpace[ i ] - vPosition_WorldSpace );
         distanceSqr = distanceSqr * distanceSqr;
        
         vec3 L = normalize( uLighDirection_WorldSpace[ i ] );
         float cosTheta = clamp( dot( N, -L ), 0, 1 );
        
         vec3 R = reflect( L, N );
         float cosAlpha = clamp( dot( E, R ), 0, 1 );
        
        //color.xyz = visibility * ( color.xyz + 
	 	//uMaterialAmbient + 
	 	//textureColor * uMaterialDiffuse.xyz * uLightColor[ i ] * uLightPower[ i ] * cosTheta / distanceSqr +
	 	//uMaterialSpecular * uLightColor[ i ] * uLightPower[ i ] * pow(cosAlpha,5) / distanceSqr );
        float power = 10.0;
        color.xyz = ( color.xyz + 
	 	textureColor * uMaterialAmbient + 
	 	textureColor * uMaterialDiffuse.xyz * uLightColor[ i ] * power * cosTheta / distanceSqr +
	 	uMaterialSpecular * uLightColor[ i ] * power * pow(cosAlpha,5) / distanceSqr );
        }
   // color.rgb = vec3( vUV, 1 );
    color.a = uMaterialDiffuse.a;
    }