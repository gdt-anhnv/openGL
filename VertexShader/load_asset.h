#ifndef _LOAD_ASSET_H_
#define _LOAD_ASSET_H_

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "assimp\vector3.h"

#include <iostream>
#include <vector>

struct Point
{
	float x;
	float y;
	float z;

	Point(float _x, float _y, float _z) :
		x(_x), y(_y), z(_z) {}

	double Distance(const Point& p)
	{
		return std::sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y) + (z - p.z) * (z - p.z));
	}
};

class aiMesh;
class AssimpMesh
{
private:
	std::vector<unsigned int> indices;
	GLuint indices_buffer;
	std::vector<glm::vec3> vertices;
	GLuint vertex_buffer;
	std::vector<glm::vec2> uvs;
	GLuint uv_buffer;
	std::vector<glm::vec3> normals;
	GLuint normal_buffer;
public:
	AssimpMesh();
	~AssimpMesh();

	void BuildMesh(const aiMesh*);
	void CreateBuffer();
	void Draw();
	void ClearBuffer();
	void GetBoundMesh(Point& min, Point& max);
	void MoveMesh(double dx, double dy, double dz);
};

class AssimpRep
{
private:
	std::string path;
	std::vector<AssimpMesh*> meshes;
	GLuint vertex_ids;

public:
	AssimpRep(const char* fp);
	~AssimpRep();

	void LoadAssimp();
	void CreateBuffer();
	void Draw();
	void ClearBuffer();
	void GetBoundModel(Point& min, Point& max);
	void MoveModel(double dx, double dy, double dz);
};
#endif