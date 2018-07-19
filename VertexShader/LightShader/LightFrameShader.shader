#version 330 core

uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 object_color;
uniform vec3 view_position;

//input
in vec3 normal;
in vec3 frag_pos;

// Ouput data
out vec4 color;

void main()
{
	//ambient
	float ambient_str = 0.1;
	vec3 ambient_vec = ambient_str * light_color;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_color;

	//specular
	float specular_str = 0.4;
	vec3 view_dir = normalize(view_position - frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_str * spec * light_color;

	//result
	vec3 ret = (ambient_vec + diffuse + specular) * object_color;
	color = vec4(ret, 1.0);
}