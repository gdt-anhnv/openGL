#ifndef _ENTITY_H_
#define _ENTITY_H_

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include <iostream>
#include <vector>

class Entity
{
private:
	float position[3];
	std::vector<std::vector<unsigned short>> indices;
	std::vector<std::vector<glm::vec3>> vertices;
	std::vector<std::vector<glm::vec2>> uvs;
	std::vector< std::vector<glm::vec3>> normals;
public:
	Entity();
	~Entity();

	void SetPosition(float x, float y, float z);
	std::vector<GLuint> GetVertexBuffer();
	std::vector<GLuint> GetTextureBuffer();
	std::vector<GLuint> GetNormalBuffer();

	std::vector<int> GetDataSize() const;
	void GetExtremePoints(glm::vec3& min, glm::vec3& max) const;
};

#endif