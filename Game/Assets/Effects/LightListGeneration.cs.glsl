#version 430

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

layout ( std430, binding = 0 ) readonly buffer lightSSBO
    {
    Light data[];
    }LightSSBO;

layout ( std430, binding = 1 ) writeonly buffer lightListSSBO
    {
    uint data[];
    }LightListSSBO;

void main()
    {
    }