#ifndef _LOAD_ASSET_H_
#define _LOAD_ASSET_H_

#include <GL\glew.h>
#include <glm\glm.hpp>
#include "assimp\vector3.h"

#include <iostream>
#include <vector>

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
};
#endif