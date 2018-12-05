#include "draw_line_program.h"
#include "settings.h"
#include "DataStructures/singleton.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

DrawLineProgram::DrawLineProgram(GLint pi) :
	program_id(pi),
	vertex_position_id(-1),
	mvp_id(-1),
	mvp_matrix(-1),
	vertex_array_id(-1),
	entities()
{
}

DrawLineProgram::~DrawLineProgram()
{
}

void DrawLineProgram::AddDLEntity(const DLEntBuffer & ent)
{
	entities.push_back(ent);
}

void DrawLineProgram::PreDrawing()
{
	vertex_position_id = glGetAttribLocation(program_id, "aPos");
	mvp_id = glGetAttribLocation(program_id, "MVP");

	glGenBuffers(1, &mvp_matrix);

	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);
}

void DrawLineProgram::Draw()
{
	glUseProgram(program_id);
	glm::mat4 model_matrix = glm::mat4(1.0);
	for (auto iter = entities.begin(); iter != entities.end(); iter++)
	{
		glEnableVertexAttribArray(vertex_position_id);
		glBindBuffer(GL_ARRAY_BUFFER, iter->vertex_buffer);
		glVertexAttribPointer(
			vertex_position_id,    // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glEnableVertexAttribArray(mvp_id);
		glEnableVertexAttribArray(mvp_id + 1);
		glEnableVertexAttribArray(mvp_id + 2);
		glEnableVertexAttribArray(mvp_id + 3);
		glm::mat4 MVP = Singleton<Settings>::GetInstance()->projection_matrix *
			Singleton<Settings>::GetInstance()->view_matrix * model_matrix;
		glBindBuffer(GL_ARRAY_BUFFER, mvp_matrix);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, &MVP[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(mvp_id, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
		glVertexAttribPointer(mvp_id + 1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
		glVertexAttribPointer(mvp_id + 2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
		glVertexAttribPointer(mvp_id + 3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
		glVertexAttribDivisor(mvp_id, 1);
		glVertexAttribDivisor(mvp_id + 1, 1);
		glVertexAttribDivisor(mvp_id + 2, 1);
		glVertexAttribDivisor(mvp_id + 3, 1);

		glDrawArrays(GL_LINES, 0, iter->size);
	}

	glDisableVertexAttribArray(vertex_position_id);
	glDisableVertexAttribArray(mvp_id);
	glDisableVertexAttribArray(mvp_id + 1);
	glDisableVertexAttribArray(mvp_id + 2);
	glDisableVertexAttribArray(mvp_id + 3);
}

void DrawLineProgram::PostDrawing()
{
	glDeleteProgram(program_id);
	glDeleteVertexArrays(1, &vertex_array_id);
}
