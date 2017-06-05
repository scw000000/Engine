#version 430

#define BLUR_SHIFT 2

uniform sampler2D   uSSAOTex;
uniform vec2        uScreenSize;

in vec2 vUV;

out vec3 oColor;
    
void main()
    {
    float sum = 0.0f;
    for( int i = -BLUR_SHIFT; i <= BLUR_SHIFT; ++i )
        {
        for( int j = -BLUR_SHIFT; j <= BLUR_SHIFT; ++j )
            {
            sum += texture( uSSAOTex, ( gl_FragCoord.xy + vec2( i, j ) ) / uScreenSize ).r; 
            }
        }
    oColor = vec3( sum / 25.f );

    }

