#ifndef _WINDOW_WRAP_H_
#define _WINDOW_WRAP_H_

#include <GLFW/glfw3.h>

struct GLFWWindowWrap
{
	GLFWwindow* glfw_window;

	GLFWWindowWrap(GLFWwindow* w) : glfw_window(w) {}
	~GLFWWindowWrap()
	{
		glfwTerminate();
	}
};

#endif