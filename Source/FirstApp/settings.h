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

class PickingObject;
class Settings
{
public:
	glm::vec3 view_position;
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	GLFWwindow* window;
	PickingObject* picking_object_manager;

	Settings();

	~Settings();
};

#endif