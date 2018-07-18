#version 330 core

uniform vec3 object_color;

//input
in vec3 frag_pos;

// Ouput data
out vec4 color;

void main()
{
	color = vec4(object_color, 1.0f);
}