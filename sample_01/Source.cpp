#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "window_wrap.h"
#include "shader.h"
#include "load_asset.h"
#include "view_port.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>


using namespace glm;

static glm::mat4 SetupCamera();

static const float HEIGHT_SCREEN = 768.0f;
static const float WIDTH_SCREEN = 1366.0f;

//#define USED_TEXTURE
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

	GLFWWindowWrap glfw_wrap(glfwCreateWindow(1366, 768, "Sample 01", nullptr, nullptr));
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
	glfwSetInputMode(glfw_wrap.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//setup camera
	GLuint program_id = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	GLuint light_id = glGetUniformLocation(program_id, "LightPosition_worldspace");
	GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
	GLuint view_matrix_id = glGetUniformLocation(program_id, "V");
	GLuint model_matrix_id = glGetUniformLocation(program_id, "M");

	ViewPort vp = ViewPort(glfw_wrap.glfw_window);
	vp.SetupCenterPnt(0.0f, 0.0f, 0.0f);
	vp.SetupCoordinate(4.0 * std::sqrt(3.0), 0.0, 0.0);
	vp.SetupMouseCoor();

#ifdef USED_TEXTURE
	GLuint Texture = loadDDS("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\monkey_head\\uvmap.DDS");
	GLuint TextureID = glGetUniformLocation(program_id, "myTextureSampler");
#endif

	//create objects
	AssimpRep* assimp_rep = new AssimpRep("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\Monkey_head\\suzanne.obj");
	assimp_rep->LoadAssimp();

	//Assimp load buffer
	assimp_rep->CreateBuffer();

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		glUseProgram(program_id);
		vp.MouseInput();
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &vp.mvp[0][0]);
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &vp.view_matrix[0][0]);
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &vp.model_matrix[0][0]);

#ifdef USED_TEXTURE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);
#endif
		glm::vec3 light_pos = glm::vec3(-4.0f, -4.0f, -4.0f);
		glUniform3f(light_id, light_pos.x, light_pos.y, light_pos.z);

		//draw here
		assimp_rep->Draw();
		//end draw here

		glfwSwapBuffers(glfw_wrap.glfw_window);
		glfwPollEvents();
	} while (glfwGetKey(glfw_wrap.glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		0 == glfwWindowShouldClose(glfw_wrap.glfw_window));

	glDeleteProgram(program_id);
	assimp_rep->ClearBuffer();

	delete assimp_rep;

	return 0;
}

//#define TEST
glm::mat4 SetupCamera()
{
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), WIDTH_SCREEN / HEIGHT_SCREEN, 0.1f, 3000.0f);

	glm::mat4 view = glm::lookAt(glm::vec3(4.0f, 4.0f, 4.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0, 1.0, 0.0));

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 mvp = projection * view * model;
	return mvp;
}