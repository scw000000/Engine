#version 430

#define MAXIMUM_LIGHTS_SUPPORTED 4096u

#define TILE_SIZE 16u

#define LIGHT_TYPE_UNDEFINED   0u
#define LIGHT_TYPE_POINT       1u
#define LIGHT_TYPE_DIRECTIONAL 2u

#define MAX_SHININESS 8192.0f

struct Light
{
    //--------------------------------------------------------------
    vec3   m_Color;
    uint   m_Type;
    //--------------------------------------------------------------
    vec3   m_PositionVS;
    uint   m_Enabled;
    //--------------------------------------------------------------
    vec3   m_DirectionVS;   // for directional light
    float  m_HalfConeAngle; // for spot light
    //--------------------------------------------------------------
    // reference http://www.ogre3d.org/tikiwiki/-Point+Light+Attenuation
    // for point and spot light
    vec4   m_Attenuation;   
    //--------------------------------------------------------------
   // float  m_Padding[3];
    //--------------------------------------------------------------
   // bool    Selected;

};
   
layout ( std430, binding = 0 ) readonly buffer lightIdxListSSBO
    {
    uint data[]; // a 1D array, length = AVERAGE_OVERLAP_LIGHTS_PER_TILE * tileNum.x * tileNum.y
    }LightIdxListSSBO;
    
layout ( std430, binding = 1 ) readonly buffer lightIdxGridSSBO
    {
    uint data[]; // array contains 2 uint per tile, ( globalStoreOffset, IdxCount )
    }LightIdxGridSSBO;
    
layout ( std430, binding = 2 ) readonly buffer lightPropsSSBO
    {
    Light data[]; // a 1D array, length = MAXIMUM_LIGHTS_SUPPORTED
    }LightPropsSSBO;

uniform sampler2D   uDepthTex;
uniform sampler2D   uMRT0;
uniform sampler2D   uMRT1;
uniform uvec2       uTileNum;
uniform vec2       uHalfSizeNearPlane;
uniform mat4        uProj;

in vec2 vUV;

out vec4 oColor;

// input: [ 0, 1 ].xyz
vec3 ToViewSpace( vec3 p )
    {
    p = p * 2.0 - vec3( 1.0, 1.0, 1.0 ); // to NDC space
    p.xy = uHalfSizeNearPlane * p.xy;
    vec3 eyeDir = vec3( p.xy, -1 );
    float positiveDepthVS = uProj[3][2] / ( p.z + uProj[2][2] ); 
    return eyeDir * positiveDepthVS;
    }

vec3 GetNormal( vec4 mrt0 )
    {
    return mrt0.xyz;
    }
    
vec3 GetAlbedo( vec4 mrt1 )
    {
    return mrt1.xyz;
    }
    
float GetGlossiness( vec4 mrt0 )
    {
	return mrt0.w;
    }
    
float GetMetalness( vec4 mrt1 )
    {
	return mrt1.w;
    }
    
float GetShininess( float glossiness )
    {
	return pow( MAX_SHININESS, glossiness );
    }
    
vec3 GetDiffuse( vec3 albedo, float metalness )
    {
   // return vec3( 1.0, 0.0, 0.0 );
	return albedo * ( 1.0 - metalness );
    }

vec3 GetSpecular( vec3 albedo, float metalness )
    {
	return mix( vec3( 0.04 ), albedo, metalness );
    }
    
float GetSpecularNormalizeFactor( float shininess )
    {
	return ( shininess + 2.0 ) / 8.0;
    }
    
vec3 GetFresnel( vec3 lightDir, vec3 halfway, vec3 specular )
    {
    float eDotN = clamp( dot( lightDir, halfway ), 0.0, 1.0 );
	return dot( specular, vec3( 1.0 ) ) > 0 ? specular + ( 1 - specular ) * exp2( -( 5.55473f * eDotN + 6.98316f) * eDotN ) : vec3( 0 );
    }

vec3 GetDistribution( vec3 halfway, vec3 normal, float shininess)
    {
	return vec3( exp( ( shininess + 0.775f ) * ( max( dot( halfway, normal ), 0.0f ) - 1) ) );
    }
    
vec3 CalcLight( uint lightIdx, vec3 meshPosVS, vec3 normal, vec3 diffuse, vec3 specular, float specularNormalFac, float shininess )
    {
    Light light = LightPropsSSBO.data[ lightIdx ];
    // float distance, 
    float dist = 0.0;
    float distSqr = 0.0;
	float luminosity = 0.0;
    vec3 viewDir = -meshPosVS; 
    vec3 lightDir = vec3( 0.0 ); 
    
    switch ( light.m_Type )
        {
        case LIGHT_TYPE_POINT:
            lightDir = light.m_PositionVS - meshPosVS;
            distSqr = dot( lightDir, lightDir );
            dist = sqrt ( distSqr );
            lightDir = normalize( lightDir );
            luminosity = 1.0 / dot( light.m_Attenuation.xyz, vec3( 1.0, dist, distSqr ) );
            break;
        case LIGHT_TYPE_DIRECTIONAL:
            return vec3( 0.0, 1.0, 0.0 );  
            lightDir = light.m_DirectionVS;
            dist = -1.0;
            luminosity = 1.0;
            break;
        default:
            return vec3( 0.0, 0.0, 0.0 );  
        };
    
    // not directional light and outside of range of light
    if( dist >= 0.0 && dist > light.m_Attenuation.w )
        {
        return vec3( 0.0, 0.0, 0.0 );
        } 
        
    float nDotL = dot( lightDir, normal );
    
    if( nDotL <= 0.0 )
        {
        return vec3( 0.0, 0.0, 0.0 );
        }
    
    vec3 halfway = normalize( lightDir + viewDir );
    specular = specularNormalFac 
               // * GetDistribution( halfway, normal, shininess );
				* GetFresnel( lightDir, halfway, specular );
    //* luminosity+ specular
    return max( ( diffuse + specular ) * ( nDotL * luminosity ), vec3( 0.0 ) ) * light.m_Color.rgb;
    }
    
void main()
    {
    // sample depth texture
    float depth = texture( uDepthTex, vUV ).r;
    
    // transfrom depth back to view space
    vec3 meshPosVS = ToViewSpace( vec3( vUV, depth ) );
    
    vec4 mrt0 = texture( uMRT0, vUV );
    vec4 mrt1 = texture( uMRT1, vUV );
    
    // retrieve normal vector
    vec3 normal = GetNormal( mrt0 );
    vec3 albedo = GetAlbedo( mrt1 );
    
    float glossiness = GetGlossiness( mrt0 );
    float metalness = GetMetalness( mrt1 );
    
    float shininess = GetShininess( glossiness );
    
    vec3 diffuse = GetDiffuse( albedo, metalness );
    vec3 specular = GetSpecular( albedo, metalness );
    
    float specularNormalFac = GetSpecularNormalizeFactor( shininess );
    
    // get light gird index
    uvec2 tileCoord = uvec2( gl_FragCoord.xy / TILE_SIZE );
    tileCoord = min( tileCoord, uTileNum - uvec2( 1u, 1u ) );
    uint tileIdx = tileCoord.x + tileCoord.y * uTileNum.x;
    // get light list offset and count
    uint listOffset = LightIdxGridSSBO.data[ tileIdx * 2u ];
    uint listLength = LightIdxGridSSBO.data[ tileIdx * 2u + 1u ];

    oColor = vec4( 0.0, 0.0, 0.0, 1.0 );

    // calculate each light in light list
    for( uint i = 0; i < listLength; ++i )
        {
       // if( LightIdxListSSBO.data[ listOffset + i ] > 0 )
        //    {
          //  oColor.xyz = vec3( 0.0, 1.0, 0.0 );
        //    break;
        //    }
        //Light light = LightPropsSSBO.data[ LightIdxListSSBO.data[ listOffset + i ] ];
        oColor.xyz += CalcLight(    LightIdxListSSBO.data[ listOffset + i ], 
                                meshPosVS, 
                                normal, 
                                diffuse, 
                                specular, 
                                specularNormalFac, 
                                shininess );
    
        }
    oColor.xyz += albedo * 0.2;
   // if( listLength == 2 )
  //      {
    //    oColor.xyz = vec3( 0.0, 1.0, 0.0 );
   //     }
  //  oColor.xyz += vec3( 0.2 );
  // oColor.xyz = diffuse;
   // oColor = vec4( vUV * 2.0 - vec2( 1.0, 1.0), 0.0, 1.0 );
  // oColor = vec4( meshPosVS.xyz, 1.0 );
    oColor.a = 1.0;
    }

