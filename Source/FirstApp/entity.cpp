#include "entity.h"
#include "common/objloader.hpp"

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <list>
#include <vector>

using namespace glm;

Entity::Entity() :
	position{ 0.0, 0.0, 0.0 },
	indices(),
	vertices(),
	uvs(),
	normals()
{
	if (!loadAssImp("Model//suzanne.obj", indices, vertices, uvs, normals))
		throw int(1);
}

Entity::~Entity()
{
}

void Entity::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
}

std::vector <GLuint> Entity::GetVertexBuffer()
{
	std::vector <GLuint> vb = std::vector <GLuint>();
	auto trans = glm::vec3(position[0], position[1], position[2]);
	for (auto iter = vertices.begin(); iter != vertices.end(); iter++)
	{
		for (auto sub_iter = iter->begin(); sub_iter != iter->end(); sub_iter++)
			sub_iter->operator+=(trans);
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, iter->size() * sizeof(glm::vec3), &(*iter)[0], GL_STATIC_DRAW);
		vb.push_back(vertexbuffer);
	}

	return vb;
}

std::vector <GLuint> Entity::GetTextureBuffer()
{
	std::vector <GLuint> uvb = std::vector <GLuint>();
	for (auto iter = uvs.begin(); iter != uvs.end(); iter++)
	{
		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, iter->size() * sizeof(glm::vec2), &(*iter)[0], GL_STATIC_DRAW);
		uvb.push_back(uvbuffer);
	}

	return uvb;
}

std::vector <GLuint> Entity::GetNormalBuffer()
{
	std::vector <GLuint> nb = std::vector <GLuint>();
	for (auto iter = normals.begin(); iter != normals.end(); iter++)
	{
		GLuint normalbuffer;
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, iter->size() * sizeof(glm::vec3), &(*iter)[0], GL_STATIC_DRAW);
		nb.push_back(normalbuffer);
	}

	return nb;
}

std::vector<int> Entity::GetDataSize() const
{
	std::vector<int> size = std::vector<int>();
	for (auto iter = vertices.begin(); iter != vertices.end(); iter++)
		size.push_back(iter->size());
	return size;
}

void Entity::GetExtremePoints(glm::vec3 & min, glm::vec3 & max) const
{
	min = vertices.front().front();
	max = vertices.front().front();

	for (auto iter = vertices.begin(); iter != vertices.end(); iter++)
	{
		for (auto sub_iter = iter->begin(); sub_iter != iter->end(); sub_iter++)
		{
			if (min.x > sub_iter->x)
				min.x = sub_iter->x;
			if (min.y > sub_iter->y)
				min.y = sub_iter->y;
			if (min.z > sub_iter->z)
				min.z = sub_iter->z;
			if (max.x < sub_iter->x)
				max.x = sub_iter->x;
			if (max.y < sub_iter->y)
				max.y = sub_iter->y;
			if (max.z < sub_iter->z)
				max.z = sub_iter->z;
		}
	}
}
