#version 430

#define MAXIMUM_LIGHTS_PER_TILE 1024

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

struct TileFrustum
    {
    vec4 m_Planes[ 4 ];
    };    

layout ( std430, binding = 0 ) coherent buffer lightCountSSBO
    {
    uint count;
    }LightCountSSBO;
    
layout ( std430, binding = 1 ) writeonly buffer lightMapSSBO
    {
    uint list[]; // a 1D array, length = MAX_LIGHT_SUPPORTED
    uint grid[]; // a 2D array contains 2 uint per tile
    }LightMapSSBO;
    
layout ( std430, binding = 2 ) readonly buffer lightPropsSSBO
    {
    Light data[];
    }LightPropsSSBO;

layout ( std430, binding = 3 ) readonly buffer tileFrustumSSBO
    {
    TileFrustum data[];
    }TileFrustumSSBO;
   
uniform sampler2D  uDepthTex;

shared float sMinDepth;
shared float sMaxDepth;
shared float sLocalIdxCount;
shared float sGlobalStoreOffset;
shared float sLocalIdxList[ MAXIMUM_LIGHTS_PER_TILE ];

void main()
    {
    }