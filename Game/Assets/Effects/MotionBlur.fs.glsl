#version 430
#define MAX_SAMPLE_LENGTH 100

uniform sampler2D   uMotionVectorTex;
uniform sampler2D   uLightingTex;
uniform float       uBlurScale;

in vec2 vUV;

out vec3 oColor;
    
void main()
    {
    vec2 motionVector = texture( uMotionVectorTex, vUV ).xy;
    // motionVector = ( motionVector - 0.5f ) * 2.0f;
    int sampleNum = int( length( motionVector * uBlurScale * textureSize( uLightingTex, 0 ) ) );
    sampleNum = clamp( sampleNum, 1, MAX_SAMPLE_LENGTH );
    vec3 sum = vec3( 0.0f );
    for( int i = 0; i < sampleNum; ++i )
        {
        sum += texture( uLightingTex, vUV + ( motionVector * ( float( i ) / float( sampleNum ) - 0.5f ) ) ).xyz;
        // sum += texture( uLightingTex, gl + ( motionVector * ( i / sampleNum - 0.5f ) ) ).xyz;
        }
    sum /= sampleNum;
    // sum = texture( uLightingTex, vUV ).xyz;
    oColor = sum; 
    }

