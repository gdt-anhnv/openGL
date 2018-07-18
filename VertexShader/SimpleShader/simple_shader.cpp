#include "simple_shader.h"
#include "../shader.h"

#define MODEL_MATRIX		"model"
#define VIEW_MATRIX			"view"
#define PROJECTION_MATRIX	"projection"
#define OBJECT_COLOR		"object_color"

SimpleShader::SimpleShader(const char* vs, const char* fs) :
	program_id(GLuint()),
	vertex_shader_file(std::string(vs)),
	frame_shader_file(std::string(fs))
{
	program_id = LoadShaders(vs, fs);
}

SimpleShader::~SimpleShader() {}

void SimpleShader::UseShader()
{
	glUseProgram(program_id);
}

void SimpleShader::SetModelMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, MODEL_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void SimpleShader::SetViewMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, VIEW_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void SimpleShader::SetProjectionMatrix(const glm::mat4 & mat)
{
	glUniformMatrix4fv(glGetUniformLocation(program_id, PROJECTION_MATRIX), 1, GL_FALSE, &mat[0][0]);
}

void SimpleShader::SetObjectColor(const GLfloat & r, const GLfloat & g, const GLfloat & b)
{
	glUniform3f(glGetUniformLocation(program_id, OBJECT_COLOR), r, g, b);
}
