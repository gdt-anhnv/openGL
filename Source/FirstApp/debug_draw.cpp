#include "debug_draw.h"
#include "DataStructures/singleton.h"
#include "settings.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <iostream>

void MyDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
}

void MyDebugDraw::reportErrorWarning(const char * warningString)
{
	std::cout << warningString << std::endl;
}

void MyDebugDraw::draw3dText(const btVector3 & location, const char * textString)
{
}

void MyDebugDraw::setDebugMode(int debugMode)
{
	debug_mode = debugMode;
}

int MyDebugDraw::getDebugMode() const
{
	return 3;
}

void MyDebugDraw::drawContactPoint(const btVector3 & PointOnB, const btVector3 & normalOnB, btScalar distance, int lifeTime, const btVector3 & color)
{
}
