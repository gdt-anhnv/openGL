#version 330 core

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 object_color;

//input
in vec3 normal;
in vec3 frag_pos;

// Ouput data
out vec4 color;

void main()
{
	//ambient
	float ambient_str = 0.2;
	vec3 ambient_vec = ambient_str * light_color;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	//result
	vec3 ret = (ambient_vec + diffuse) * object_color;
	color = vec4(ret, 1.0);
}