#include "orientation_quat.h"
#include "glm/gtc/quaternion.hpp"

#include <iostream>

OrientationQuat::OrientationQuat(float _x, float _y, float _z, float _w) :
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
}

OrientationQuat::~OrientationQuat()
{
}

glm::mat3 OrientationQuat::ToMatrix() const
{
	glm::mat3 mat;

	mat[0][0] = 1.0f - 2.0f * (y * y + z * z);
	mat[0][1] = 2 * (x * y - z * w);
	mat[0][2] = 2 * (x * z + y * w);

	mat[1][0] = 2 * (x * y + z * w);
	mat[1][1] = 1.0f - 2.0f * (x * x + z * z);
	mat[1][2] = 2 * (y * z - x * w);

	mat[2][0] = 2 * (x * z - y * w);
	mat[2][1] = 2 * (y * z + w * x);
	mat[2][2] = 1.0f - 2.0f * (x * x + y * y);

	return std::move(mat);
}

void OrientationQuat::Setup(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

void OrientationQuat::Rotate(float _x, float _y, float _z, float _w)
{
	glm::quat quat1;
	quat1.x = x;
	quat1.y = y;
	quat1.z = z;
	quat1.w = w;

	glm::quat quat2;
	quat2.x = _x;
	quat2.y = _y;
	quat2.z = _z;
	quat2.w = _w;

	glm::quat ret_quat = quat1 * quat2;
	ret_quat = glm::normalize(ret_quat);

	x = ret_quat.x;
	y = ret_quat.y;
	z = ret_quat.z;
	w = ret_quat.w;
}
