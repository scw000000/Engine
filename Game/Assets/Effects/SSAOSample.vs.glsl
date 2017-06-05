#version 430

layout ( location = 0 ) in vec2 iVertexPosMS;
layout ( location = 1 ) in vec2 iUV;

out vec2 vUV;
out vec2 vViewRay;

void main()
    {
    gl_Position = vec4( iVertexPosMS, -1.0 , 1.0 );
    vUV = iUV;
    // transform from [ 0, 1 ] to [ -1, 1 ]
    vViewRay = ( iUV - vec2( 0.5f ) ) * 2.0f;
    }

