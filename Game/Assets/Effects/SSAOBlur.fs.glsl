#version 430

#define MAX_SAMPLE_COUNT 300

uniform sampler2D   uSSAOTex;

// ratio between ( x, y ) and z value, y = tan( pCamera->GetFrustum().m_FovY / 2.0f );
uniform vec2 uHalfSizeNearPlane; 
uniform vec2 uNoiseTexSampleScale;
uniform mat4 uProjectMat;

uniform uint uSampleCount;
uniform vec3 uSampleVectors[ MAX_SAMPLE_COUNT ];

in vec2 vUV;
in vec2 vViewRay;

out vec4 oColor;
    
void main()
    {
    vec3 normal = GetNormal( texture( uMRT0, vUV ) );
    vec3 randomVec = texture( uNoiseTex, vUV * uNoiseTexSampleScale ).rgb;
    vec3 tangent = normalize( randomVec - normal * ( dot( normal, randomVec ) ) );
    vec3 bitanget = cross( tangent, normal );
    mat3 tbn = mat3( tangent, bitanget, normal );
    
    float depth = texture( uDepthTex, vUV ).r;
    vec3 cuurrentPointVS = DepthToViewSpaceVec( depth );
    
    uint iterNum = clamp( uSampleCount, 0u, MAX_SAMPLE_COUNT );
    float occlusion = 0.0f;
    for( int i = 0; i < iterNum; ++i )
        {
        vec3 samplePointVS = ( tbn * uSampleVectors[ i ] * 10.0f ) + cuurrentPointVS;
        vec4 p = uProjectMat * vec4( samplePointVS, 1.0 );
        p.xy /= p.w;
        p = ( p + vec4( 1.0 ) ) * 0.5;
        float projectDepth = DepthToViewSpaceDepth( texture( uDepthTex, p.xy ).r );
        occlusion += ( projectDepth > samplePointVS.z )? 1.0f : 0.0f;
        }
    occlusion = pow( 1.0 - occlusion / ( float( iterNum ) ), 2.0f );
    
    oColor = vec4( occlusion );
    //if( iterNum == 100 )
    //    {
    //    oColor = vec4( 1, 0, 0, 1 );
    //    }
    
    }

