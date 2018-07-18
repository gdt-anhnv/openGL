#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 frag_pos;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = projection * view * model * vec4(pos, 1.0);

	frag_pos = vec3(model * vec4(pos, 1.0));
}
