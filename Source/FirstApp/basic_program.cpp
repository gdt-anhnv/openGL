#include "basic_program.h"
#include "common/texture.hpp"
#include "settings.h"
#include "DataStructures/singleton.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

BasicProgram::BasicProgram(GLint pi) :
	program_id(pi),
	entities(std::list<EntBuffer>()),
	texture_id(-1),
	light_id(-1),
	view_matrix_id(-1),
	model_matrix_id(-1),
	mvp_id(-1),
	vertex_position_id(-1),
	vertex_uv_id(-1),
	vertex_normal_id(-1),
	vertex_array_id(-1),
	texture(-1),
	mvp_matrix(-1),
	light_position{}
{
}

BasicProgram::~BasicProgram()
{
}

void BasicProgram::AddEntity(const EntBuffer & eb)
{
	entities.push_back(eb);
}

void BasicProgram::SetLightPosition(GLfloat x, GLfloat y, GLfloat z)
{
	light_position[0] = x;
	light_position[1] = y;
	light_position[2] = z;
}

void BasicProgram::PreDrawing()
{
	texture_id = glGetUniformLocation(program_id, "myTextureSampler");
	light_id = glGetUniformLocation(program_id, "LightPosition_worldspace");
	view_matrix_id = glGetUniformLocation(program_id, "V");
	model_matrix_id = glGetUniformLocation(program_id, "M");

	mvp_id = glGetAttribLocation(program_id, "MVP");
	vertex_position_id = glGetAttribLocation(program_id, "vertexPosition_modelspace");
	vertex_uv_id = glGetAttribLocation(program_id, "vertexUV");
	vertex_normal_id = glGetAttribLocation(program_id, "vertexNormal_modelspace");

	glGenBuffers(1, &mvp_matrix);

	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	// Load the texture
	texture = loadDDS("Model\\uvmap.DDS");

}

void BasicProgram::Draw()
{
	glUseProgram(program_id);
	glUniform3f(light_id, light_position[0], light_position[1], light_position[2]);
	glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &Singleton<Settings>::GetInstance()->view_matrix[0][0]);
	glm::mat4 model_matrix = glm::mat4(1.0);
	glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &model_matrix[0][0]);

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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(texture_id, 0);

		glEnableVertexAttribArray(vertex_uv_id);
		glBindBuffer(GL_ARRAY_BUFFER, iter->texture_buffer);
		glVertexAttribPointer(
			vertex_uv_id,        // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                   // size
			GL_FLOAT,            // type
			GL_FALSE,            // normalized?
			0,                   // stride
			(void*)0             // array buffer offset
		);

		glEnableVertexAttribArray(vertex_normal_id);
		glBindBuffer(GL_ARRAY_BUFFER, iter->normal_buffer);
		glVertexAttribPointer(
			vertex_normal_id,                 // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
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

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, iter->size);
	}

	glDisableVertexAttribArray(vertex_position_id);
	glDisableVertexAttribArray(vertex_uv_id);
	glDisableVertexAttribArray(vertex_normal_id);
	glDisableVertexAttribArray(mvp_id);
	glDisableVertexAttribArray(mvp_id + 1);
	glDisableVertexAttribArray(mvp_id + 2);
	glDisableVertexAttribArray(mvp_id + 3);

}

void BasicProgram::PostDrawing()
{
	ClearBuffer();
	glDeleteProgram(program_id);
	glDeleteVertexArrays(1, &vertex_array_id);
}

void BasicProgram::ClearBuffer()
{
	for (auto iter = entities.begin(); iter != entities.end(); iter++)
	{
		glDeleteBuffers(1, &iter->vertex_buffer);
		glDeleteBuffers(1, &iter->texture_buffer);
		glDeleteBuffers(1, &iter->normal_buffer);
	}

	entities.clear();
}
