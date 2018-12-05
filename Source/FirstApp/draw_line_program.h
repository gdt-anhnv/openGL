#ifndef _DRAW_LINE_PROGRAM_H_
#define _DRAW_LINE_PROGRAM_H_

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

struct DLEntBuffer
{
	GLuint vertex_buffer;
	int size;

	DLEntBuffer(GLuint vb, int s) :
		vertex_buffer(vb),
		size(s)
	{}

	DLEntBuffer(const DLEntBuffer& ent) :
		vertex_buffer(std::move(ent.vertex_buffer)),
		size(ent.size)
	{}

	~DLEntBuffer() {}
};



class DrawLineProgram
{
private:
	GLint program_id;
	GLuint vertex_position_id;
	GLuint mvp_id;
	GLuint mvp_matrix;
	GLuint vertex_array_id;
	std::list<DLEntBuffer> entities;

public:
	DrawLineProgram(GLint pi);
	~DrawLineProgram();

	void AddDLEntity(const DLEntBuffer&);

	void PreDrawing();
	void Draw();
	void PostDrawing();
};

#endif