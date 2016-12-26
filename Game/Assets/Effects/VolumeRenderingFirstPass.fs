// for raycasting
#version 400

in vec3 oColor;
out vec4 oFragColor;


void main()
    {
    oFragColor = vec4( oColor, 1.0);
    }
