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

	glfwSetInputMode(glfw_wrap.glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_CULL_FACE);

	//setup camera
	GLuint program_id = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
	glm::mat4 mvp = SetupCamera();

	GLuint vertex_arr_id;
	glGenVertexArrays(1, &vertex_arr_id);
	glBindVertexArray(vertex_arr_id);

	AssimpRep* assimp_rep = new AssimpRep("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\BB8\\BB8.FBX");
	assimp_rep->LoadAssimp();

	//Assimp load buffer
	assimp_rep->CreateBuffer();

	glUseProgram(program_id);
	GLuint light_id = glGetUniformLocation(program_id, "LightPosition_worldspace");

	do
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//use shader
		glUseProgram(program_id);
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mvp[0][0]);

		glm::vec3 lightPos = glm::vec3(400.0, 400.0, 400.0);
		glUniform3f(light_id, lightPos.x, lightPos.y, lightPos.z);

		//draw here
		assimp_rep->Draw();
		//end draw here

		glfwSwapBuffers(glfw_wrap.glfw_window);
		glfwPollEvents();
	} while (glfwGetKey(glfw_wrap.glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		0 == glfwWindowShouldClose(glfw_wrap.glfw_window));

	glDeleteProgram(program_id);
	glDeleteBuffers(1, &vertex_arr_id);
	assimp_rep->ClearBuffer();

	delete assimp_rep;

	return 0;
}

//#define TEST
glm::mat4 SetupCamera()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH_SCREEN / HEIGHT_SCREEN, 0.1f, 3000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(-800.0, -30.0, 1400.0),
		glm::vec3(30.0, 30.0, 30.0),
		glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;
	return mvp;
}
