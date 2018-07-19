#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal_source;
layout(location = 2) in vec2 texture_source;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 frag_pos;
out vec2 texture_coord;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
	gl_Position = projection * view * model * vec4(pos, 1.0);

	normal = mat3(transpose(inverse(model))) * normal_source;
	frag_pos = vec3(model * vec4(pos, 1.0));
	texture_coord = texture_source;
}
