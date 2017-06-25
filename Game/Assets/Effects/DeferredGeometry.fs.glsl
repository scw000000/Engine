#version 400

in vec3 vTangentVS;
in vec3 vBitangentVS;
in vec3 vNormalVS;
in vec2 vUV;
in vec4 vCurrPosCS;
in vec4 vPrevPosCS;

uniform sampler2D   uAlbedoTex;

uniform sampler2D   uNormalMapTex;
uniform uint        uUseNormalMap;

layout(location = 0) out vec4 oMRT0; // normal + roughness
layout(location = 1) out vec4 oMRT1; // albedo + metalness
layout(location = 2) out vec2 oMRT2; // velocity

void main(){
    if( uUseNormalMap == 1u )
        {
        mat3 tbn = mat3( normalize( vTangentVS ), normalize( vBitangentVS ), normalize( vNormalVS ) );
        oMRT0.xyz = tbn * texture( uNormalMapTex, vUV ).rgb; 
        }
    else
        {
        oMRT0.xyz = normalize( vNormalVS );
        }
    
    // transform from [+-1] to [0,1]
    oMRT0.xyz = ( oMRT0.xyz + 1.0 ) * 0.5;
    
    oMRT0.w = 0.9;
    
    oMRT1.xyz = texture( uAlbedoTex, vUV ).rgb;
    oMRT1.w = 0.01;
    
    // current pixel position in normal space [0, 1]
    vec2 currPos = ( ( vCurrPosCS.xy / vCurrPosCS.w ) + 1.0f ) * 0.5f;
    vec2 prevPos = ( ( vPrevPosCS.xy / vPrevPosCS.w ) + 1.0f ) * 0.5f;
    
    // oMRT2 = ( ( currPos - prevPos ) + 1.0f ) * 0.5f;
    oMRT2 = ( currPos - prevPos );
}