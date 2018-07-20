#ifndef _SIMPLE_SHADER_H_
#define _SIMPLE_SHADER_H_

#include <iostream>
#include <string>

#include <GL\glew.h>
#include <glm\glm.hpp>

class SimpleShader
{
private:
	GLuint program_id;

public:
	SimpleShader(const char* vs, const char* fs);
	~SimpleShader();

	void UseShader();

	void SetModelMatrix(const glm::mat4& mat);
	void SetViewMatrix(const glm::mat4& mat);
	void SetProjectionMatrix(const glm::mat4& mat);

	void SetObjectColor(const GLfloat& r, const GLfloat& g, const GLfloat& b);
};

#endif