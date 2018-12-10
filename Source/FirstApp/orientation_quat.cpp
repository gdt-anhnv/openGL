#include "orientation_quat.h"

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
	mat[0][1] = 2 * (x * y + z * w);
	mat[0][2] = 2 * (x * z - z * y);
	mat[1][0] = 2 * (x * y - z * w);
	mat[1][1] = 1.0f - 2.0f * (x * x + z * z);
	mat[1][2] = 2 * (y * z + x * w);
	mat[2][0] = 2 * (x * z + y * w);
	mat[2][1] = 2 * (y * z - w * x);
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
