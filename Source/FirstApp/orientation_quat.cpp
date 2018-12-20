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
	glm::quat quat;
	quat.x = x;
	quat.y = y;
	quat.z = z;
	quat.w = w;
	return std::move(glm::transpose(glm::mat3_cast(quat)));
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
