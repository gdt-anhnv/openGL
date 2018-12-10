#include "camera_entity.h"

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

CameraEntity::CameraEntity() :
	position{},
	//direction{},
	//up{},
	view_at{},
	orientation_quat{ 0.0f, 0.0f, 0.0f, 0.0f },
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

//void CameraEntity::SetDirection(float x, float y, float z)
//{
//	direction[0] = x;
//	direction[1] = y;
//	direction[2] = z;
//}
//
//void CameraEntity::SetUpDir(float x, float y, float z)
//{
//	up[0] = x;
//	up[1] = y;
//	up[2] = z;
//}

void CameraEntity::SetViewAt(float x, float y, float z)
{
	view_at[0] = x;
	view_at[1] = y;
	view_at[2] = z;
}

void CameraEntity::InitOrientation(float * direction, float * up)
{
	glm::vec3 oz = glm::vec3(direction[0], direction[1], direction[2]);
	glm::vec3 oy = glm::vec3(up[0], up[1], up[2]);

	glm::vec3 ox = glm::cross(oy, oz);

	float qw = 0.5f * std::sqrt(ox[0] * ox[0] + oy[1] * oy[1] + oz[2] * oz[2] + 1);
	float qx = (oz[1] - oy[2]) / (4.0f * qw);
	float qy = (oz[0] - ox[2]) / (4.0f * qw);
	float qz = (oy[0] - ox[1]) / (4.0f * qw);

	orientation_quat.Setup(qx, qy, qz, qw);
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

	view_mat[3][0] = position[0];
	view_mat[3][1] = position[1];
	view_mat[3][2] = position[2];
	view_mat[3][3] = 1.0f;

	return view_mat;
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
