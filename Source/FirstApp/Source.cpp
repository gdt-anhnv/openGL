#include "entity.h"
#include "basic_program.h"
#include "settings.h"
#include "DataStructures/singleton.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"

// Include standard headers
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>


void MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (GLFW_MOUSE_BUTTON_LEFT == button && GLFW_PRESS == action)
	{
		double xpos = 0.0;
		double ypos = 0.0;
		glfwGetCursorPos(Singleton<Settings>::GetInstance()->window, &xpos, &ypos);
		std::cout << xpos << " - " << ypos << std::endl;
	}
}

int main(void)
{
	Singleton<Settings>::AssignInstance(new Settings());

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
	Singleton<Settings>::GetInstance()->window = glfwCreateWindow(1024, 768, "OpenGL", NULL, NULL);
	if (NULL == Singleton<Settings>::GetInstance()->window) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Singleton<Settings>::GetInstance()->window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetMouseButtonCallback(Singleton<Settings>::GetInstance()->window, MouseEvent);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Singleton<Settings>::GetInstance()->window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders

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

	Singleton<Settings>::GetInstance()->view_matrix = glm::lookAt(
		glm::vec3(center.x, center.y, center.z + length),
		center,
		glm::vec3(0.0, 1.0, 0.0));

	Singleton<Settings>::GetInstance()->projection_matrix =
		glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	BasicProgram* bp = new BasicProgram(LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader"));
	bp->SetLightPosition(400.0f, 400.0f, 400.0f);
	bp->AddEntity(vertexbuffers.front());

	bp->PreDrawing();

	BasicProgram* bp2 = new BasicProgram(LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader"));
	bp2->SetLightPosition(-400.0f, 400.0f, 400.0f);
	bp2->AddEntity(vertexbuffers.back());

	bp2->PreDrawing();

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bp->Draw();
		bp2->Draw();

		// Swap buffers
		glfwSwapBuffers(Singleton<Settings>::GetInstance()->window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(Singleton<Settings>::GetInstance()->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(Singleton<Settings>::GetInstance()->window) == 0);

	// Cleanup VBO and shader
	for (auto iter = vertexbuffers.begin(); iter != vertexbuffers.end(); iter++)
	{
		glDeleteBuffers(1, &iter->vertex_buffer);
		glDeleteBuffers(1, &iter->texture_buffer);
		glDeleteBuffers(1, &iter->normal_buffer);
	}

	bp->PostDrawing();
	bp2->PostDrawing();

	delete bp;
	delete bp2;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	for (auto iter = ents.begin(); iter != ents.end(); iter++)
		delete *iter;

	ents.clear();

	Singleton<Settings>::destroy();

	return 0;
}

