#include "light_shader.h"
#include "../shader.h"

#define MODEL_MATRIX		"model"
#define VIEW_MATRIX			"view"
#define PROJECTION_MATRIX	"projection"
#define LIGHT_COLOR			"light_color"
#define LIGHT_DIRECTION		"light.direction"
#define LIGHT_POSITION		"light.position"
#define LIGHT_AMBIENT		"light.ambient"
#define LIGHT_DIFFUSE		"light.diffuse"
#define LIGHT_SPECULAR		"light.specular"
#define OBJECT_COLOR		"object_color"
#define VIEW_POSITION		"view_position"

#define MATERIAL_DIFFUSE	"material.diffuse"
#define MATERIAL_SPECULAR	"material.specular"
#define MATERIAL_SHININESS	"material.shininess"

LightShader::LightShader(const char* vs, const char* fs) :
	program_id(GLuint()),
	vertex_shader_file(std::string(vs)),
	frame_shader_file(std::string(fs))
{
	program_id = LoadShaders(vs, fs);
}

LightShader::~LightShader() {}

void LightShader::UseShader()
{
	glUseProgram(program_id);
}

void LightShader::SetModelMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, MODEL_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void LightShader::SetViewMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, VIEW_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void LightShader::SetProjectionMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, PROJECTION_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void LightShader::SetLightDirection(const GLfloat & x, const GLfloat & y, const GLfloat & z)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_DIRECTION), x, y, z);
}

void LightShader::SetLightColor(const GLfloat & r, const GLfloat & g, const GLfloat & b)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_COLOR), r, g, b);
}

void LightShader::SetLightPosition(const GLfloat & x, const GLfloat & y, const GLfloat & z)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_POSITION), x, y, z);
}

void LightShader::SetObjectColor(const GLfloat & r, const GLfloat & g, const GLfloat & b)
{
	glUniform3f(glGetUniformLocation(program_id, OBJECT_COLOR), r, g, b);
}

void LightShader::SetViewPosition(const GLfloat & x, const GLfloat & y, const GLfloat & z)
{
	glUniform3f(glGetUniformLocation(program_id, VIEW_POSITION), x, y, z);
}

void LightShader::SetLightAmbient(const double & v0, const double & v1, const double & v2)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_AMBIENT), v0, v1, v2);
}

void LightShader::SetLightDiffuse(const double & v0, const double & v1, const double & v2)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_DIFFUSE), v0, v1, v2);
}

void LightShader::SetLightSpecular(const double & v0, const double & v1, const double & v2)
{
	glUniform3f(glGetUniformLocation(program_id, LIGHT_SPECULAR), v0, v1, v2);
}

void LightShader::SetMaterialSpecular(int val)
{
	glUniform1i(glGetUniformLocation(program_id, MATERIAL_SPECULAR), val);
}

void LightShader::SetMaterialDiffuse(int val)
{
	glUniform1i(glGetUniformLocation(program_id, MATERIAL_DIFFUSE), val);
}

void LightShader::SetMaterialShininess(const double & val)
{
	glUniform1f(glGetUniformLocation(program_id, MATERIAL_SHININESS), val);
}
