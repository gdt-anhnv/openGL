#ifndef _LIGHT_SHADER_H_
#define _LIGHT_SHADER_H_

#include <iostream>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

class LightShader
{
private:
	GLuint program_id;

	std::string vertex_shader_file;
	std::string frame_shader_file;

public:
	LightShader(const char* vs, const char* fs);
	~LightShader();

	void UseShader();

	void SetModelMatrix(const glm::mat4& mat);
	void SetViewMatrix(const glm::mat4& mat);
	void SetProjectionMatrix(const glm::mat4& mat);

	void SetLightColor(const GLfloat& r, const GLfloat& g, const GLfloat& b);
	void SetLightPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z);
	void SetObjectColor(const GLfloat& r, const GLfloat& g, const GLfloat& b);
	void SetViewPosition(const GLfloat& x, const GLfloat& y, const GLfloat& z);
};

#endif