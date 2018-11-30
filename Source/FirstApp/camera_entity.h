#ifndef _CAMERA_ENTITY_H_
#define _CAMERA_ENTITY_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class CameraEntity
{
private:
	float position[3];
	float direction[3];
	float up[3];
public:
	CameraEntity();
	~CameraEntity();

	void SetPosition(float x, float y, float z);
	void SetDirection(float x, float y, float z);
	void SetUpDir(float x, float y, float z);

	glm::mat4 GetViewMatrix();
};

#endif