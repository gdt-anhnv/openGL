#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "window_wrap.h"
#include "shader.h"
#include "load_asset.h"

using namespace glm;

static void DrawSomething(const GLuint& vertex_buffer, int num_ver);
static glm::mat4 SetupCamera();

static const float HEIGHT_SCREEN = 768.0f;
static const float WIDTH_SCREEN = 1366.0f;

int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW \n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWWindowWrap glfw_wrap(glfwCreateWindow(1024, 768, "Sample 01", nullptr, nullptr));
	if (nullptr == glfw_wrap.glfw_window)
	{
		fprintf(stderr, "Failed to open GLFW window \n");
		getchar();
		return -1;
	}

	glfwMakeContextCurrent(glfw_wrap.glfw_window);

	if (GLEW_OK != glewInit())
	{
		fprintf(stderr, "Failed to initialize GLEW \n");
		getchar();
		return -1;
	}

	//init VAO
	GLuint ver_ids;
	glGenVertexArrays(1, &ver_ids);
	glBindVertexArray(ver_ids);
	//end VAO

	glfwSetInputMode(glfw_wrap.glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//setup camera
	GLuint program_id = LoadShaders("SimpleTransform.vertexshader", "SingleColor.fragmentshader");
	GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
	glm::mat4 mvp = SetupCamera();

	//Init for drawing
	int num_ver = 0;
	GLuint vertex_buffer = AssimpRep::LoadAsset("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\BB8\\bb8.FBX", num_ver);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//use shader
		glUseProgram(program_id);
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		//draw here
		DrawSomething(vertex_buffer, num_ver);
		//end draw here

		glfwSwapBuffers(glfw_wrap.glfw_window);
		glfwPollEvents();
	} while (glfwGetKey(glfw_wrap.glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		0 == glfwWindowShouldClose(glfw_wrap.glfw_window));

	return 0;
}

void DrawSomething(const GLuint& vertex_buffer, int num_ver)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//color
	{
		GLuint color_buffer;
		glGenBuffers(1, &color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		GLfloat* color = new GLfloat[num_ver]();
		for (int i = 0; i < num_ver; i++)
			color[i] = i % 4;
		glBufferData(GL_ARRAY_BUFFER, num_ver, color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glDrawArrays(GL_TRIANGLES, 0, num_ver);
	glDisableVertexAttribArray(0);
}

glm::mat4 SetupCamera()
{
	aiVector3t<float> min;
	aiVector3t<float> max;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH_SCREEN / HEIGHT_SCREEN, 0.1f, 3000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(800.0, 30.0, 1400.0),
		glm::vec3(30.0, 30.0, 30.0),
		glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;

	return mvp;
}
