#version 330 core

// Ouput data
out vec4 color;

struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//input data
in vec3 frag_pos;
in vec3 normal;
in vec2 texture_coord;

uniform vec3 view_position;
uniform Material material;
uniform Light light;

void main()
{
	//ambient
	vec3 ambient = light.ambient * texture(material.diffuse, texture_coord).rgb;

	//diffuse
	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light.position - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, texture_coord).rgb;

	//specular
	vec3 view_dir = normalize(view_position - frag_pos);
	vec3 reflection_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflection_dir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	//result
	vec3 ret = ambient + diffuse + specular;
	color = vec4(ret, 1.0);
}