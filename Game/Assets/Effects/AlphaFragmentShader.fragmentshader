#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 TotalDiffuseColor;
in vec3 TotalSpecularColor;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float Alpha;

void main()
	{
	// Output color = color of the texture at the specified UV
	color.rgb = texture( myTextureSampler, UV ).rgb * TotalDiffuseColor + TotalSpecularColor;
	color.a = Alpha;
	}