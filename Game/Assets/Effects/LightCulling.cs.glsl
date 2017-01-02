#version 430
#define MAXIMUM_LIGHTS_SUPPORTED 8u
#define MAXIMUM_LIGHTS_PER_TILE 1024u
#define AVERAGE_OVERLAP_LIGHTS_PER_TILE 200u

#define TILE_SIZE 16u

#define LIGHT_TYPE_UNDEFINED   0u
#define LIGHT_TYPE_POINT       1u
#define LIGHT_TYPE_DIRECTIONAL 2u

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
    vec4   m_Attenuation;   // for point and spot light
    //--------------------------------------------------------------
   // float  m_Padding[3];
    //--------------------------------------------------------------
   // bool    Selected;

};

// frustum facing inward
struct TileFrustum
    {
    vec4 m_Planes[ 4 ];
    };    
                            
layout ( std430, binding = 0 ) coherent buffer lightIdxCountSSBO
    {
    uint data;
    }LightIdxCountSSBO;
    
layout ( std430, binding = 1 ) buffer lightIdxListSSBO
    {
    uint data[]; // a 1D array, length = AVERAGE_OVERLAP_LIGHTS_PER_TILE * tileNum.x * tileNum.y
    }LightIdxListSSBO;
    
layout ( std430, binding = 2 ) writeonly buffer lightIdxGridSSBO
    {
    uint data[]; // array contains 2 uint per tile, ( globalStoreOffset, IdxCount )
    }LightIdxGridSSBO;
    
layout ( std430, binding = 3 ) readonly buffer lightPropsSSBO
    {
    Light data[]; // a 1D array, length = std::min( lights.size(), MAXIMUM_LIGHTS_SUPPORTED )
    }LightPropsSSBO;

layout ( std430, binding = 4 ) readonly buffer tileFrustumSSBO
    {
    TileFrustum data[];
    }TileFrustumSSBO;
 
uniform sampler2D   uDepthTex;
uniform mat4        uProj;
uniform uvec2       uScreenSize;
uniform uint        uValidLightNum;
uniform uint        uInvokePerGroup = TILE_SIZE * TILE_SIZE;

shared uint sTileIndex;
shared uint sGlobalIdxListLength;

shared uint sMinDepthU;
shared uint sMaxDepthU;
shared float sMinDepth;
shared float sMaxDepth;

shared uint sGlobalStoreOffset;

shared uint sLocalIdxCount;
shared uint sLocalIdxList[ MAXIMUM_LIGHTS_PER_TILE ];

shared vec4 sNearFarPlanes[ 2 ];

bool IsSphereAbovePlane( vec3 centerVS, float radius, vec4 plane )
    {
    return dot( vec4( centerVS, 1.0 ), plane ) + radius >= 0.0;
    }
    
bool IsSphereInsideFrustum( vec3 centerVS, 
                            float radius, 
                            vec4 planes[4], 
                            float nearPlaneZVS, 
                            float farPlaneZVS )
    {
    if( centerVS.z - radius > nearPlaneZVS || farPlaneZVS > centerVS.z + radius )
        {
        return false;
        }
    for ( int i = 0; i < 4; i++ )
        {
        if ( !IsSphereAbovePlane( centerVS, radius, planes[i] ) )
            {
            return false;
            }
        }
    return true;
    }
    
void AppendLocalIdxList( uint lightIdx )
    {
    uint origIdx = atomicAdd( sLocalIdxCount, 1 );
    if( origIdx < MAXIMUM_LIGHTS_PER_TILE )
        {
        sLocalIdxList[ origIdx ] = lightIdx;
        }
    }

layout ( rgba16 ) writeonly uniform image2D debugTex;
    
layout ( local_size_x = TILE_SIZE, local_size_y = TILE_SIZE, local_size_z = 1 ) in;

void main()
    {
    // gl_NumWorkGroups​
    // gl_WorkGroupID​
    // gl_LocalInvocationID​
    // gl_GlobalInvocationID​
    if( gl_LocalInvocationIndex == 0 )
        {
        sMinDepthU = 0xFFFFFFFFu;
        sMaxDepthU = 0u;
        sLocalIdxCount = 0u;
        sGlobalStoreOffset = 0u;
        sTileIndex = gl_WorkGroupID.x + gl_NumWorkGroups.x * gl_WorkGroupID.y;
        sGlobalIdxListLength = gl_NumWorkGroups.x * gl_NumWorkGroups.y * gl_NumWorkGroups.z * AVERAGE_OVERLAP_LIGHTS_PER_TILE;
        }
    
    barrier();
    // test min max depth
    
    if( gl_GlobalInvocationID.x < uScreenSize.x && gl_GlobalInvocationID.y < uScreenSize.y ) // valid depth test position
        {
        vec2 tc = vec2( gl_GlobalInvocationID.xy ) / vec2( uScreenSize );
        uint depth = floatBitsToUint( texture( uDepthTex, tc ).r );
        atomicMin( sMinDepthU, depth );
        atomicMax( sMaxDepthU, depth );
        }
               
    barrier();
    // set min max depth and far near plane

    if( gl_LocalInvocationIndex == 0 )
        {
        // to NDC space
        sMinDepth = uintBitsToFloat( sMinDepthU ) * 2.0 - 1.0;
        sMaxDepth = uintBitsToFloat( sMaxDepthU ) * 2.0 - 1.0;
        
        sMinDepth = -uProj[3][2] / ( sMinDepth + uProj[2][2] );
        sMaxDepth = -uProj[3][2] / ( sMaxDepth + uProj[2][2] );
        
        // if sMinDepth = -N, sMaxDepth = -F
        // ( 0, 0, -N, 1 ) dot vec4( 0.0, 0.0, -1.0, sMinDepth ) = N - N = 0
        // ( 0, 0, -F, 1 ) dot vec4( 0.0, 0.0, 1.0, -sMinDepth ) = -F + F = 0
        sNearFarPlanes = vec4[]( vec4( 0.0, 0.0, -1.0, sMinDepth ), vec4( 0.0, 0.0, 1.0, -sMaxDepth ) );
        }
        
    barrier();

    // testing light culling
    for( uint i= gl_LocalInvocationIndex; i < uValidLightNum; i += uInvokePerGroup )
        {
        if( LightPropsSSBO.data[ i ].m_Enabled == 1u )
            {
            switch( LightPropsSSBO.data[ i ].m_Type )
                {
                case LIGHT_TYPE_POINT:
                    if( IsSphereInsideFrustum( LightPropsSSBO.data[ i ].m_PositionVS,
                                               LightPropsSSBO.data[ i ].m_Attenuation.w,
                                               TileFrustumSSBO.data[ sTileIndex ].m_Planes,
                                               sMinDepth,
                                               sMaxDepth
                                                ) )
                        {
                        AppendLocalIdxList( i );
                        }
                    break;
                case LIGHT_TYPE_DIRECTIONAL:
                    AppendLocalIdxList( i );
                    break;
                };
            }
        }
        
    barrier();
    
    if( gl_LocalInvocationIndex == 0 )
        {
        sLocalIdxCount = min( sLocalIdxCount, MAXIMUM_LIGHTS_PER_TILE );
        sGlobalStoreOffset = atomicAdd( LightIdxCountSSBO.data, sLocalIdxCount );
        }
    
    barrier();
        
    for( uint i= gl_LocalInvocationIndex; i < sLocalIdxCount && sGlobalStoreOffset + i < sGlobalIdxListLength; i += uInvokePerGroup )
        {
        LightIdxListSSBO.data[ sGlobalStoreOffset + i ] = sLocalIdxList[ i ];
        }
        
    if( gl_LocalInvocationIndex == 0 )
        {
        LightIdxGridSSBO.data[ sTileIndex * 2     ] = sGlobalStoreOffset;
        LightIdxGridSSBO.data[ sTileIndex * 2 + 1 ] = sLocalIdxCount;
        }  

    barrier();
    
    if( gl_GlobalInvocationID.x < uScreenSize.x && gl_GlobalInvocationID.y < uScreenSize.y ) // valid depth test position
        {
         imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 0.0, 0.0, 0.0, 1.0 ) );
            
        if( sLocalIdxCount == 1 )
            {
            imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 0.0, 1.0, 0.0, 1.0 ) );
            }
        else if( sLocalIdxCount == 0 )
            {
            imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 0.0, 0.0, 0.0, 1.0 ) );
            }
        if( sLocalIdxCount == 2 )
            {
            // sGlobalIdxListLength = 510600
            if( sGlobalStoreOffset + 1 >= sGlobalIdxListLength )
                {
                imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 0.0, 0.0, 1.0, 1.0 ) );
                
                }
            imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 1.0, 0.0, 0.0, 1.0 ) );
            }
            
       // if( ( gl_WorkGroupID.x % 2u ) == 1 && ( gl_WorkGroupID.y % 2u ) == 1 )
      //      {
      //      imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 1.0, 0.0, 0.0, 1.0 ) );
          
      //      }  
    //  imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( 0.0, 0.0, 0.0, 1.0 ) );

      //  imageStore( debugTex, ivec2( gl_GlobalInvocationID.xy ), vec4( vec3( gl_WorkGroupID.xyz ) / vec3( gl_NumWorkGroups.xyz ), 1.0 ) );
            
        }
    }
