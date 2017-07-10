#version 430

#define MAXIMUM_LIGHTS_SUPPORTED 4096u

#define TILE_SIZE 16u

#define LIGHT_TYPE_UNDEFINED   0u
#define LIGHT_TYPE_POINT       1u
#define LIGHT_TYPE_DIRECTIONAL 2u

#define MAX_SHININESS 25.0f

#define GAMMA 2.2f
#define INVERSE_GAMMA 1.0f / 2.2f

#define PI 3.1415926535897932384626433832795

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
uniform sampler2D   uSSAOBlurTex;
uniform uint        uUseSSAO;
uniform uvec2       uTileNum;
// ratio between ( x, y ) and z value, y = tan( pCamera->GetFrustum().m_FovY / 2.0f );
uniform vec2       uHalfSizeNearPlane; 
uniform mat4        uProj;

in vec2 vUV;

layout(location = 0) out vec3 oColor; 
layout(location = 1) out vec3 oLightThreshold; 

// input: [ 0, 1 ].xyz
vec3 ToViewSpace( vec3 p )
    {
    p = p * 2.0 - vec3( 1.0, 1.0, 1.0 ); // to NDC space
    float eyeDirScale = uProj[3][2] / ( p.z + uProj[2][2] ); 
    p.xy *= uHalfSizeNearPlane;
    p.z = -1.0;
    return p * eyeDirScale;
    }

vec3 GetNormal( vec4 mrt0 )
    {
    return ( mrt0.xyz - vec3( 0.5 ) ) * 2.0;
    }
    
vec3 GetAlbedo( vec4 mrt1 )
    {
    return mrt1.xyz;
    }
    
float GetRoughness( vec4 mrt0 )
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
    
vec3 GetFresnel( float cosTheta, vec3 f0 )
    {
    //float eDotN = clamp( dot( lightDir, halfway ), 0.0, 1.0 );
	//return dot( specular, vec3( 1.0 ) ) > 0.0 ? specular + ( vec3( 1.0f ) - specular ) * exp2( -( 5.55473f * eDotN + 6.98316f) * eDotN ) : vec3( 0.0 );
  // return specular;
   // return min( specular.b, min( specular.r, specular.g ) ) > 0.0 ? specular + ( vec3( 1.0f ) - specular ) * exp2( -( 5.55473f * eDotN + 6.98316f) * eDotN ) : vec3( 0.0 );
    
    return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
    }

float GetDistribution( vec3 halfway, vec3 normal, float roughness )
    {
    float a      = roughness * roughness;
    float aSqr     = a * a;
    float ndoth  = max( dot( normal, halfway ), 0.0);
    float ndothSqr = ndoth * ndoth;
	
    float nom   = aSqr;
    float denom = ( ndothSqr * ( aSqr - 1.0) + 1.0 );
    denom = PI * denom * denom;
	
    return nom / denom;
    }
    
float GetLuminosity( vec3 lightLoc, vec3 lightAttenuation, vec3 meshLoc )
    {
    vec3 lightDir = ( lightLoc - meshLoc );
    float distSqr = dot( lightDir, lightDir );
    return 1.0f / dot( lightAttenuation, vec3( 1, sqrt( distSqr ), distSqr ) );
    }
    
float GeometrySchlickGGX( float ndotv, float roughness )
    {
    float r = ( roughness + 1.0 );
    float k = ( r * r ) / 8.0;

    float nom   = ndotv;
    float denom = ndotv * ( 1.0 - k ) + k;
	
    return nom / denom;
    }

float GetGeometry( vec3 N, vec3 V, vec3 L, float roughness )
    {
    float ndotv = max( dot( N, V ), 0.0 );
    float ndotl = max( dot( N, L ), 0.0 );
    float ggx2  = GeometrySchlickGGX( ndotv, roughness );
    float ggx1  = GeometrySchlickGGX( ndotl, roughness );
	
    return ggx1 * ggx2;
    }
    
vec3 CalcLight( uint lightIdx, vec3 meshPosVS, vec3 normal, vec3 albedo, float metalness, float roughness )
    {
    Light light = LightPropsSSBO.data[ lightIdx ];
    // float distance, 
    float dist = 0.0;
    float distSqr = 0.0;
	float luminosity = 0.0;
    vec3 viewDir = -meshPosVS; 
    viewDir = normalize( viewDir );
    vec3 lightDir = vec3( 0.0 ); 
    
    switch ( light.m_Type )
        {
        case LIGHT_TYPE_POINT:
            lightDir = light.m_PositionVS - meshPosVS;
            if( dot( lightDir, lightDir ) > light.m_Attenuation.w * light.m_Attenuation.w )
                {
                return vec3( 0.0, 0.0, 0.0 );
                }
            lightDir = normalize( lightDir );
            luminosity = GetLuminosity( light.m_PositionVS, light.m_Attenuation.xyz, meshPosVS );
            break;
        case LIGHT_TYPE_DIRECTIONAL:
            lightDir = -light.m_DirectionVS;
            dist = -1.0;
            luminosity = 1.0;
            break;
        default:
            return vec3( 0.0, 0.0, 0.0 );  
        };
    
    vec3 halfway = normalize( lightDir + viewDir );
    float ndotl = max( dot( lightDir, normal ), 0.0 );
    
    vec3 f0 = vec3( 0.04 ); 
    f0      = mix( f0, albedo, metalness );
    vec3 fresnel = GetFresnel( max( dot( halfway, viewDir ), 0.0 ), f0 );
    vec3 specular = GetDistribution( halfway, normal, roughness )
                * GetGeometry( normal, viewDir, lightDir, roughness )
				* fresnel / 
                ( 4 * max( dot( normal, viewDir ), 0.0 ) * ndotl + 0.001 );
                
    vec3 kS = fresnel;
    vec3 kD = vec3( 1.0 ) - kS;
  
    kD *= 1.0 - metalness;	
    return ( kD * albedo / PI + specular ) * light.m_Color * luminosity * ndotl;
    //* luminosity+ specular
    // return max( ( diffuse + specular ) * ( ndotl * luminosity ), vec3( 0.0 ) ) * light.m_Color.rgb;
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
    
    float roughness = GetRoughness( mrt0 );
    float metalness = GetMetalness( mrt1 );
    
    // float shininess = GetShininess( glossiness );
    
    // vec3 diffuse = GetDiffuse( albedo, metalness );
    // vec3 specular = GetSpecular( albedo, metalness );
    
    // float specularNormalFac = GetSpecularNormalizeFactor( shininess );
    
    // get light gird index
    uvec2 tileCoord = uvec2( gl_FragCoord.xy / TILE_SIZE );
    tileCoord = min( tileCoord, uTileNum - uvec2( 1u, 1u ) );
    uint tileIdx = tileCoord.x + tileCoord.y * uTileNum.x;
    // get light list offset and count
    uint listOffset = LightIdxGridSSBO.data[ tileIdx * 2u ];
    uint listLength = LightIdxGridSSBO.data[ tileIdx * 2u + 1u ];

    vec3 outputColor = vec3( 0.0, 0.0, 0.0 );

    // calculate each light in light list
    for( uint i = 0; i < listLength; ++i )
        {
       // if( LightIdxListSSBO.data[ listOffset + i ] > 0 )
        //    {
          //  outputColor = vec3( 0.0, 1.0, 0.0 );
        //    break;
        //    }
        //Light light = LightPropsSSBO.data[ LightIdxListSSBO.data[ listOffset + i ] ];
        
        outputColor += CalcLight(    LightIdxListSSBO.data[ listOffset + i ], 
                                meshPosVS, 
                                normal, 
                                albedo, 
                                metalness, 
                                roughness );
    
        }
       
    float occlusion = 1.0f;
    if( uUseSSAO == 1u )
        {
        occlusion = texture( uSSAOBlurTex, vUV ).r;
        }
    outputColor += albedo * 0.04 * occlusion;
    
    // outputColor = clamp( outputColor, 0.0f, 50.f );
    
    oLightThreshold = vec3( 0.0f );
    if( dot( outputColor.rgb, vec3( 0.2126, 0.7152, 0.0722 ) ) > 0.03f )
        {
        oLightThreshold = outputColor;
        }
    
    // doing the tone mapping
    // outputColor = outputColor / ( outputColor + vec3( 0.187 ) ) * 1.035;
    
    // outputColor = outputColor / (outputColor + vec3(1.0));
    // outputColor = pow(outputColor, vec3(1.0/GAMMA)); 

    // outputColor = pow( outputColor, GAMMA_VALUE );
   // if( listLength == 2 )
  //      {
    //    outputColor = vec3( 0.0, 1.0, 0.0 );
   //     }
  //  outputColor += vec3( 0.2 );
  // outputColor = diffuse;
   // outputColor = vec4( vUV * 2.0 - vec2( 1.0, 1.0), 0.0, 1.0 );
   // outputColor = ToViewSpace( vec3( vUV, depth ) );
    oColor = outputColor;
    // oColor = vec4( outputColor, 1.0f );
    }

