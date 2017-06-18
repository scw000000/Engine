#version 430
#define GAMMA 2.2f

uniform sampler2D   uLightinTex;

in vec2 vUV;

out vec3 oColor;
    
void main()
    {
    vec3 sampleColor = texture( uLightinTex, vec2( gl_FragCoord.xy ) / vec2( textureSize( uLightinTex, 0 ) ) ).xyz;
    sampleColor = sampleColor / ( sampleColor + vec3( 1.0 ) );
    oColor = pow( sampleColor, vec3( 1.0 / GAMMA ) ); 
    }

