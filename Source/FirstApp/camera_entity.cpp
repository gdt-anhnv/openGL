#include "camera_entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

CameraEntity::CameraEntity() :
	position{},
	direction{},
	up{},
	view_at{},
	mouse_position{}
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

void CameraEntity::SetViewAt(float x, float y, float z)
{
	view_at[0] = x;
	view_at[1] = y;
	view_at[2] = z;
}

glm::mat4 CameraEntity::GetViewMatrix()
{
	return glm::lookAt(glm::vec3(position[0], position[1], position[2]),
		glm::vec3(view_at[0], view_at[1], view_at[2]),
		glm::vec3(up[0], up[1], up[2]));
}

static glm::vec3 CalCamPos(const glm::vec3& axe, float delta, const glm::vec3& pos)
{
	glm::mat4 rot_mat = glm::rotate(glm::mat4(1.0f), delta, axe);
	glm::vec4 ret = rot_mat * glm::vec4(pos, 1.0);

	return glm::vec3(ret.x, ret.y, ret.z);
}

void CameraEntity::UpdateMousePos(double x, double y)
{
	if (std::abs(x - mouse_position[0]) < 1.0 && std::abs(y - mouse_position[1]) < 1.0)
		return;
	glm::vec3 cam_pos = CalCamPos(glm::vec3(up[0], up[1], up[2]),
		(mouse_position[0] - x) * 0.05, glm::vec3(position[0], position[1], position[2]));

	glm::vec3 cross_axe = glm::cross(glm::vec3(up[0], up[1], up[2]),
		glm::vec3(cam_pos.x - view_at[0], cam_pos.y - view_at[1], cam_pos.z - view_at[2]));

	glm::vec3 ult_cam_pos = CalCamPos(cross_axe, (mouse_position[1] - y) * 0.05, cam_pos);

	glm::quat quaternion;
	glm::vec3 rot_axe = glm::cross(glm::vec3(position[0] - view_at[0], position[1] - view_at[1], position[2] - view_at[2]),
		glm::vec3(ult_cam_pos.x - view_at[0], ult_cam_pos[1] - view_at[1], ult_cam_pos[2] - view_at[2]));
	quaternion.x = up[0];
	quaternion.y = up[1];
	quaternion.z = up[2];
	glm::quat up_quat = glm::rotate(quaternion, -15.0f * 3.1415f / 180.0f, glm::vec3(rot_axe.x, rot_axe.y, rot_axe.z));

	up[0] = up_quat.x;
	up[1] = up_quat.y;
	up[2] = up_quat.z;

	position[0] = ult_cam_pos.x;
	position[1] = ult_cam_pos.y;
	position[2] = ult_cam_pos.z;

	mouse_position[0] = x;
	mouse_position[1] = y;
}
