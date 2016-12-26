#version 400

in vec2 oUV;
in vec3 oPosition_WorldSpace;
in vec3 oNormal_WorldSpace;
in vec3 oEyeDirection_WorldSpace;

const int MAXIMUM_LIGHTS_SUPPORTED = 8;

uniform vec3 	uLightPosition_WorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform vec3 	uLighDirection_WorldSpace[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform vec3	uLightColor[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform float   uLightPower[ MAXIMUM_LIGHTS_SUPPORTED ];
uniform int     uLightNumber;

// Values that stay constant for the whole mesh.
uniform sampler2D   uMeshTexture;
uniform vec4 	    uMaterialDiffuse;
uniform vec3	    uMaterialAmbient;
uniform vec3	    uMaterialSpecular;

// Ouput data
out vec4 color;

void main()
    {
     vec3 N = normalize( oNormal_WorldSpace );
     vec3 E = normalize( oEyeDirection_WorldSpace );
     vec3 textureColor = texture( uMeshTexture, oUV ).rgb;
    color = vec4( 0, 0, 0, 1 );
    
   for( int i = 0; i < uLightNumber; ++i )
         {
         float distanceSqr = length( uLightPosition_WorldSpace[ i ] - oPosition_WorldSpace );
         distanceSqr = distanceSqr * distanceSqr;
        
         vec3 L = normalize( uLighDirection_WorldSpace[ i ] );
         float cosTheta = clamp( dot( N, -L ), 0, 1 );
        
         vec3 R = reflect( L, N );
         float cosAlpha = clamp( dot( E, R ), 0, 1 );
        
        color.xyz = color.xyz + 
	 	uMaterialAmbient + 
	 	textureColor * uMaterialDiffuse.xyz * uLightColor[ i ] * uLightPower[ i ] * cosTheta / distanceSqr +
	 	uMaterialSpecular * uLightColor[ i ] * uLightPower[ i ] * pow(cosAlpha,5) / distanceSqr;
        }
        
    color.a = uMaterialDiffuse.a;
    }