#include "basic_program.h"

BasicProgram::BasicProgram(GLint pi) :
	program_id(pi),
	entities(std::list<EntBuffer>()),
	texture_id(-1),
	light_id(-1),
	view_matrix_id(-1),
	model_matrix_id(-1),
	mvp_location(-1)
{
}

BasicProgram::~BasicProgram()
{
}

void BasicProgram::AddEntity(const EntBuffer & eb)
{
	entities.push_back(eb);
}

void BasicProgram::PreDrawing()
{
	texture_id = glGetUniformLocation(program_id, "myTextureSampler");
	light_id = glGetUniformLocation(program_id, "LightPosition_worldspace");
	view_matrix_id = glGetUniformLocation(program_id, "V");
	model_matrix_id = glGetUniformLocation(program_id, "M");
	mvp_location = glGetAttribLocation(program_id, "MVP");
}

void BasicProgram::Draw()
{
	for (auto iter = entities.begin(); iter != entities.end(); iter++)
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, iter->vertex_buffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(texture_id, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, iter->texture_buffer);
		glVertexAttribPointer(
			1,                   // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                   // size
			GL_FLOAT,            // type
			GL_FALSE,            // normalized?
			0,                   // stride
			(void*)0             // array buffer offset
		);

		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, iter->normal_buffer);
		glVertexAttribPointer(
			2,                                // attribute
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		int pos = glGetAttribLocation(program_id, "MVP");
		glEnableVertexAttribArray(pos);
		glEnableVertexAttribArray(pos + 1);
		glEnableVertexAttribArray(pos + 2);
		glEnableVertexAttribArray(pos + 3);
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		if (iter == vertexbuffers.begin())
			ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(2.0));
		else
			ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.5));

		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
		glBindBuffer(GL_ARRAY_BUFFER, mvp_matrix);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, &MVP[0][0], GL_STATIC_DRAW);
		glVertexAttribPointer(pos, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(0));
		glVertexAttribPointer(pos + 1, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 4));
		glVertexAttribPointer(pos + 2, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 8));
		glVertexAttribPointer(pos + 3, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4 * 4, (void*)(sizeof(float) * 12));
		glVertexAttribDivisor(pos, 1);
		glVertexAttribDivisor(pos + 1, 1);
		glVertexAttribDivisor(pos + 2, 1);
		glVertexAttribDivisor(pos + 3, 1);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, iter->size);
	}

}

void BasicProgram::PostDrawing()
{
}
