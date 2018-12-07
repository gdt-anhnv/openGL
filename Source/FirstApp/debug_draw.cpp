#include "debug_draw.h"
#include "DataStructures/singleton.h"
#include "settings.h"
#include "global_vars.h"
#include "draw_line_program.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <iostream>

static GLuint GetDLVertexBuffer(const btVector3& pos1, const btVector3& pos2)
{
	GLfloat vd[] = { pos1.x(), pos1.y(), pos1.z(), pos2.x(), pos2.y(), pos2.z() };
	GLuint vertexbuffer = -1;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vd), &vd[0], GL_STATIC_DRAW);

	return vertexbuffer;
}

void MyDebugDraw::drawLine(const btVector3 & from, const btVector3 & to, const btVector3 & color)
{
	Singleton<GlobalVars>::GetInstance()->debug_bullet_engine->AddDLEntity(
		DLEntBuffer(GetDLVertexBuffer(from, to), 2));
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
