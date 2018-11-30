#include "entity.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"
GLFWwindow* window;

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"
#include "common/controls.hpp"

// Include standard headers
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>


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
};

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Load the texture
	GLuint Texture = loadDDS("Model\\uvmap.DDS");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	std::list<Entity*> ents = std::list<Entity*>();

	{
		Entity* entity = new Entity();
		entity->SetPosition(2.0, 0.0, 0.0);
		ents.push_back(entity);
	}

	{
		Entity* entity = new Entity();
		entity->SetPosition(-2.0, 0.0, 0.0);
		ents.push_back(entity);
	}

	std::list<EntBuffer> vertexbuffers;
	glm::vec3 min = glm::vec3();
	glm::vec3 max = glm::vec3();
	ents.front()->GetExtremePoints(min, max);
	for (auto iter = ents.begin(); iter != ents.end(); iter++)
	{
		auto vb = (*iter)->GetVertexBuffer();
		auto tb = (*iter)->GetTextureBuffer();
		auto nb = (*iter)->GetNormalBuffer();
		auto sb = (*iter)->GetDataSize();
		for (int i = 0; i < vb.size(); i++)
		{
			vertexbuffers.push_back(EntBuffer(vb[i], tb[i], nb[i], sb[i]));
		}

		glm::vec3 sub_min = glm::vec3();
		glm::vec3 sub_max = glm::vec3();
		(*iter)->GetExtremePoints(sub_min, sub_max);
		if (min.x > sub_min.x)
			min.x = sub_min.x;
		if (min.y > sub_min.y)
			min.y = sub_min.y;
		if (min.z > sub_min.z)
			min.z = sub_min.z;
		if (max.x < sub_max.x)
			max.x = sub_max.x;
		if (max.y < sub_max.y)
			max.y = sub_max.y;
		if (max.z < sub_max.z)
			max.z = sub_max.z;
	}

	double length = glm::length(max - min);
	glm::vec3 center = glm::vec3(
		0.5 * (max.x + min.x),
		0.5 * (max.y + min.x),
		0.5 * (max.z + min.z));

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		//computeMatricesFromInputs();
		//glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		//glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ViewMatrix = glm::lookAt(
			glm::vec3(center.x, center.y, center.z + length),
			center,
			glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(400, 400, 400);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		for (auto iter = vertexbuffers.begin(); iter != vertexbuffers.end(); iter++)
		{
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
			glUniform1i(TextureID, 0);

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

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, iter->size);
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	for (auto iter = vertexbuffers.begin(); iter != vertexbuffers.end(); iter++)
	{
		glDeleteBuffers(1, &iter->vertex_buffer);
		glDeleteBuffers(1, &iter->texture_buffer);
		glDeleteBuffers(1, &iter->normal_buffer);
	}

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

