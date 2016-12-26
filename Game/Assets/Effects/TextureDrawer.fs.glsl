#version 400

in vec2 oUV;

out vec4 oColor;

uniform sampler2D uTexture;

void main()
    {
	oColor = vec4( texture( uTexture, oUV ).rgb, 1 );
    //oColor.gba = vec3( 0.0, 0.0, 1.0 );
    //oColor = vec4( 1.0, 0.0, 0.0, 1.0 );
    }