#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal_source;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = projection * view * model * vec4(pos, 1.0);

	normal = normal_source;
	frag_pos = vec3(model * vec4(pos, 1.0));
}
