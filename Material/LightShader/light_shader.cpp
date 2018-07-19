#include "light_shader.h"
#include "../shader.h"

#define MODEL_MATRIX		"model"
#define VIEW_MATRIX			"view"
#define PROJECTION_MATRIX	"projection"
#define LIGHT_COLOR			"light_color"
#define LIGHT_POSITION		"light_pos"
#define OBJECT_COLOR		"object_color"
#define VIEW_POSITION		"view_position"

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
