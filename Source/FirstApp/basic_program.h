#ifndef _BASIC_PROGRAM_H_
#define _BASIC_PROGRAM_H_

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <iostream>
#include <list>

struct EntBuffer
{
	GLuint vertex_buffer;
	GLuint texture_buffer;
	GLuint normal_buffer;
	int size;

	EntBuffer(GLuint vb, GLuint tb, GLuint nb, int s) :
		vertex_buffer(vb),
		texture_buffer(tb),
		normal_buffer(nb),
		size(s)
	{}

	EntBuffer(const EntBuffer& ent) :
		vertex_buffer(std::move(ent.vertex_buffer)),
		texture_buffer(std::move(ent.texture_buffer)),
		normal_buffer(std::move(ent.normal_buffer)),
		size(ent.size)
	{}

	~EntBuffer() {}
};

class BasicProgram
{
private:
	GLint program_id;
	std::list<EntBuffer> entities;

	//charactisterics
	GLuint texture_id;
	GLuint light_id;
	GLuint view_matrix_id;
	GLuint model_matrix_id;
	GLuint mvp_id;
	GLuint vertex_position_id;
	GLuint vertex_uv_id;
	GLuint vertex_normal_id;
	GLuint vertex_array_id;
	GLuint texture;
	GLuint mvp_matrix;
public:
	BasicProgram(GLint pi);
	~BasicProgram();

	void AddEntity(const EntBuffer&);

	void PreDrawing();
	void Draw(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);
	void PostDrawing();
};

#endif