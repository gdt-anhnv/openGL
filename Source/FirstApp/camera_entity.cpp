#include "camera_entity.h"

CameraEntity::CameraEntity() :
	position{},
	direction{},
	up{}
{
}

CameraEntity::~CameraEntity()
{
}

void CameraEntity::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

void CameraEntity::SetDirection(float x, float y, float z)
{
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
}

void CameraEntity::SetUpDir(float x, float y, float z)
{
	up[0] = x;
	up[1] = y;
	up[2] = z;
}

glm::mat4 CameraEntity::GetViewMatrix()
{
	return glm::lookAt(glm::vec3(position[0], position[1], position[2]),
		glm::vec3(position[0] + direction[0], position[1] + direction[1], position[2] + direction[2]),
		glm::vec3(up[0], up[1], up[2]));
}
