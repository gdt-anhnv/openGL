#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "view_port.h"

ViewPort::ViewPort(GLFWwindow* _w) :
	center_point(),
	mouse_coord(),
	camera_distance(0.0),
	phi_angle(0.0),
	theta_angle(0.0),
	window(_w),
	model_matrix(glm::mat4(1.0f)),
	view_matrix(glm::mat4(1.0f)),
	projection_matrix(glm::mat4(1.0f)),
	mvp(glm::mat4(1.0f))
{
}

ViewPort::~ViewPort()
{
}

void ViewPort::SetupCenterPnt(double x, double y, double z)
{
	center_point[0] = x;
	center_point[1] = y;
	center_point[2] = z;
}

void ViewPort::SetupCoordinate(double cd, double pa, double ta)
{
	camera_distance = cd;
	phi_angle = pa;
	theta_angle = ta;
}

void ViewPort::SetupMouseCoor(double x, double y)
{
	mouse_coord[0] = x;
	mouse_coord[1] = y;
}

void ViewPort::SetupMouseCoor()
{
	glfwGetCursorPos(window, &mouse_coord[0], &mouse_coord[1]);
}

void ViewPort::MouseInput()
{
	double mx;
	double my;
	glfwGetCursorPos(window, &mx, &my);

	double delta_x;
	double delta_y;
	delta_x = mouse_coord[0] - mx;
	delta_y = mouse_coord[1] - my;

	if (delta_x > delta_y)
		theta_angle += delta_x / 400.0;
	else
		phi_angle += delta_y / 400.0;

	mouse_coord[0] = mx;
	mouse_coord[1] = my;

	//calculate result matrix
	double view_pos[3] = { 0.0, 0.0, 0.0 };
	view_pos[0] = camera_distance * std::sin(theta_angle) * std::cos(phi_angle);
	view_pos[1] = camera_distance * std::sin(theta_angle) * std::sin(phi_angle);
	view_pos[2] = camera_distance * std::cos(theta_angle);

	double up[3] = {
		std::sin(theta_angle + 3.14 / 2.0) * std::cos(phi_angle),
		std::sin(theta_angle + 3.14 / 2.0) * std::sin(phi_angle),
		std::cos(theta_angle + 3.14 / 2.0)
	};

	view_matrix = glm::lookAt(glm::vec3(view_pos[0], view_pos[1], view_pos[2]),
		glm::vec3(center_point[0], center_point[1], center_point[2]),
		glm::vec3(up[0], up[1], up[2]));

	projection_matrix = glm::perspective(glm::radians(45.0f), 1366.0f / 768.0f, 0.1f, 100.0f);

	mvp = projection_matrix * glm::mat4(1.0f) * view_matrix;
}
