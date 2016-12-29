#version 430

struct Light
{
    //--------------------------------------------------------------
    vec4   m_Color;
    //--------------------------------------------------------------
    vec3   m_PositionVS;
    uint   m_Type;
    //--------------------------------------------------------------
    vec3   m_DirectionVS;   // for directional light
    uint   m_Enabled;
    //--------------------------------------------------------------
    vec4   m_Attenuation;   // for point and spot light
    //--------------------------------------------------------------
    float  m_HalfConeAngle; // for spot light
    float  m_Padding[3];
    //--------------------------------------------------------------
   // bool    Selected;

};