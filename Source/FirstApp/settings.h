#ifndef _SETTINGS_H_
#define _SETTINGS_H_

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

class Settings
{
public:
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	GLFWwindow* window;

	Settings() :
		view_matrix(glm::mat4(1.0f)),
		projection_matrix(glm::mat4(1.0f)),
		window(nullptr)
	{}

	~Settings() {}
};

#endif