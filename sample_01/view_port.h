#ifndef _VIEW_PORT_H_
#define _VIEW_PORT_H_

#include <glm\glm.hpp>

class GLFWwindow;
class ViewPort
{
private:
	double center_point[3];
	double mouse_coord[2];

	//spherical coordinate
	double camera_distance;
	double phi_angle;
	double theta_angle;
	//end spherical coordinate

	GLFWwindow* window;

public:
	//result
	glm::mat4 model_matrix;
	glm::mat4 view_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 mvp;

public:
	ViewPort(GLFWwindow* _w);
	~ViewPort();

	void SetupCenterPnt(double x, double y, double z);
	void SetupCoordinate(double cd, double pa, double ta);
	void SetupMouseCoor(double x, double y);
	void SetupMouseCoor();

	void MouseInput();
};

#endif