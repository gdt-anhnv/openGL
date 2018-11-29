#ifndef _ENTITY_H_
#define _ENTITY_H_

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

class Entity
{
private:
	float position[3];
	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
public:
	Entity();
	~Entity();

	void SetPosition(float x, float y, float z);
	GLuint GetVertexBuffer();
	GLuint GetColorBuffer();
	GLuint GetTextureBuffer();
	GLuint GetNormalBuffer();

	int GetDataSize() const;
};

#endif