#ifndef _ORIENTATION_QUAT_H_
#define _ORIENTATION_QUAT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrientationQuat
{
public:
	float x;
	float y;
	float z;
	float w;

	OrientationQuat(float _x, float _y, float _z, float _w);
	~OrientationQuat();

	glm::mat3 ToMatrix() const;
	void Setup(float _x, float _y, float _z, float _w);
	void Rotate(float _x, float _y, float _z, float _w);
	void RotateAbit();
};

#endif