#version 400

in vec3 vTangentVS;
in vec3 vBitangentVS;
in vec3 vNormalVS;
in vec2 vUV;

uniform sampler2D   uAlbedoTex;

uniform sampler2D   uNormalMapTex;
uniform uint        uUseNormalMap;

layout(location = 0) out vec4 oMRT0; // normal + roughness
layout(location = 1) out vec4 oMRT1; // albedo + metalness

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
        
    oMRT0.xyz = ( oMRT0.xyz + 1.0 ) * 0.5;
    
    oMRT0.w = 0.8;
    
    oMRT1.xyz = texture( uAlbedoTex, vUV ).rgb;
    oMRT1.w = 0.4;
}