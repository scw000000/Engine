#version 430
#define MAX_SAMPLE_LENGTH 100

uniform uint   uIsHorizontal;
uniform sampler2D   uSampleTex;
uniform float uFilterWeight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

in vec2 vUV;

out vec3 oColor;

void main()
    {
    vec3 sum = texture( uSampleTex, vUV ).xyz * uFilterWeight[ 0 ];
    vec2 offset = vec2( 0.0f );
    vec2 sizeDivider = 1.0f / textureSize( uSampleTex, 0 );
    for( int i = 1; i < 5; ++i )
        {
        if( uIsHorizontal == 1u )
            {
            offset = vec2( i * sizeDivider.x, 0.0f ) ;
            }
        else
            {
            offset = vec2( 0.0f, i * sizeDivider.y );
            }
            
        sum += texture( uSampleTex, vUV + offset ).xyz * uFilterWeight[ i ];
        sum += texture( uSampleTex, vUV - offset ).xyz * uFilterWeight[ i ];        
        }
   
    oColor = sum; 
    }

