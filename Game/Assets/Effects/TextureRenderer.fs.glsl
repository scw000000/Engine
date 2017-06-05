#version 400

in vec2 oUV;

out vec3 oColor;

uniform sampler2D uTexture;
uniform uint uUseSingleColor;

void main()
    {
	vec3 sampleColor = texture( uTexture, oUV ).rgb;
	// Output color = color of the texture at the specified UV
    if( uUseSingleColor == 1u )
        {
        oColor = vec3( sampleColor.r );
        }
    else
        {
        oColor = sampleColor;
        }
    }