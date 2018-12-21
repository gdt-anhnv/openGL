#include "camera_entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include <iostream>

CameraEntity::CameraEntity() :
	position{},
	//direction{},
	up_dir{},
	view_at{},
	orientation_quat{ 0.0f, 0.0f, 0.0f, 0.0f },
	mouse_position{}
{
}

CameraEntity::~CameraEntity()
{
}

void CameraEntity::InitOrientation(float* pos, float* va, float* up)
{
	position[0] = pos[0];
	position[1] = pos[1];
	position[2] = pos[2];

	view_at[0] = va[0];
	view_at[1] = va[1];
	view_at[2] = va[2];

	up_dir[0] = up[0];
	up_dir[1] = up[1];
	up_dir[2] = up[2];

	auto trans = glm::lookAt(glm::vec3(position[0], position[1], position[2]),
		glm::vec3(view_at[0], view_at[1], view_at[2]),
		glm::vec3(up_dir[0], up_dir[1], up_dir[2]));

	auto quat = glm::quat_cast(trans);

	//orientation_quat.Setup(quat.x, quat.y, quat.z, quat.w);
	orientation_quat.Setup(0.0f, -1.0f / sqrt(2.0f), 0.0f, 1.0f / sqrt(2.0f));
}

glm::mat4 CameraEntity::GetViewMatrix()
{
	glm::mat3 mat = orientation_quat.ToMatrix();
	glm::mat4 view_mat = glm::mat4(1.0f);

	view_mat[0][0] = mat[0][0];
	view_mat[0][1] = mat[0][1];
	view_mat[0][2] = mat[0][2];
	view_mat[0][3] = 0.0f;

	view_mat[1][0] = mat[1][0];
	view_mat[1][1] = mat[1][1];
	view_mat[1][2] = mat[1][2];
	view_mat[1][3] = 0.0f;

	view_mat[2][0] = mat[2][0];
	view_mat[2][1] = mat[2][1];
	view_mat[2][2] = mat[2][2];
	view_mat[2][3] = 0.0f;

	double distance_cam = std::sqrt(
		(position[0] - view_at[0]) * (position[0] - view_at[0]) +
		(position[1] - view_at[1]) * (position[1] - view_at[1]) +
		(position[2] - view_at[2]) * (position[2] - view_at[2]));
	position[0] = mat[0][2] * distance_cam;
	position[1] = mat[1][2] * distance_cam;
	position[2] = mat[2][2] * distance_cam;
	view_mat = glm::translate(view_mat, glm::vec3(-position[0], -position[1], -position[2]));

	//std::cout << view_mat[1][0] << " - " << view_mat[1][1] << " - " << view_mat[1][2] << std::endl;

	return view_mat;
}

void CameraEntity::RotateCam()
{
	orientation_quat.Rotate(
		1.0f / std::sqrt(6.0f) * std::sin(0.02f * 3.1415f / 180.0f),
		1.0f / std::sqrt(6.0f) * std::sin(0.02f * 3.1415f / 180.0f),
		1.0f / std::sqrt(6.0f) * std::sin(0.02f * 3.1415f / 180.0f),
		1.0f / std::sqrt(2.0f) * std::cos(0.02f * 3.1415f / 180.0f));

	glm::mat4 mat_view = GetViewMatrix();

	auto look_at = glm::lookAt(glm::vec3(position[0], position[1], position[2]),
		glm::vec3(view_at[0], view_at[1], view_at[2]),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 vec = glm::vec3(position[0], position[1], position[2]);
	auto up = glm::cross(vec, glm::vec3(1.0f, 0.0f, 0.0f));
	up = glm::normalize(up);
}

static glm::vec3 CalCamPos(const glm::vec3& axe, float delta, const glm::vec3& pos)
{
	glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), delta, axe);
	glm::vec4 ret = rot_mat * glm::vec4(pos, 1.0);

	return glm::vec3(ret.x, ret.y, ret.z);
}

void CameraEntity::UpdateMousePos(double x, double y)
{
}
