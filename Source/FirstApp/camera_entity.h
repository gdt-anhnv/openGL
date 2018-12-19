#ifndef _CAMERA_ENTITY_H_
#define _CAMERA_ENTITY_H_

#include "orientation_quat.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class CameraEntity
{
private:
	float position[3];
	//float direction[3];
	float up_dir[3];
	float view_at[3];

	OrientationQuat orientation_quat;

public:
	double mouse_position[2];
public:
	CameraEntity();
	~CameraEntity();

	//void SetPosition(float x, float y, float z);
	//void SetDirection(float x, float y, float z);
	//void SetUpDir(float x, float y, float z);
	//void SetViewAt(float x, float y, float z);
	void InitOrientation(float* pos, float* va, float* up);

	glm::mat4 GetViewMatrix();
	void RotateCam();
	void UpdateMousePos(double x, double y);
};

#endif