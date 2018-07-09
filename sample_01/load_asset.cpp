#include "load_asset.h"

#include "assimp\scene.h"
#include "assimp\cimport.h"
#include "assimp\postprocess.h"
#include "assimp\matrix4x4.h"

#include <iostream>
#include <GL\glew.h>
#include <vector>

AssimpMesh::AssimpMesh() :
	indices(std::vector<unsigned int>()),
	indices_buffer(GLuint()),
	vertices(std::vector<glm::vec3>()),
	vertex_buffer(GLuint()),
	uvs(std::vector<glm::vec2>()),
	uv_buffer(GLuint()),
	normals(std::vector<glm::vec3>()),
	normal_buffer(GLuint())
{
}

AssimpMesh::~AssimpMesh()
{
}

void AssimpMesh::BuildMesh(const aiMesh * mesh)
{
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		vertices.push_back(glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z));

	indices.reserve(3 * mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indices.push_back(mesh->mFaces[i].mIndices[0]);
		indices.push_back(mesh->mFaces[i].mIndices[1]);
		indices.push_back(mesh->mFaces[i].mIndices[2]);
	}

	uvs.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		uvs.push_back(glm::vec2(
			mesh->mTextureCoords[0][i].x,
			mesh->mTextureCoords[0][i].y));

	normals.reserve(mesh->mNumVertices);
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
		normals.push_back(glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z));
}

void AssimpMesh::CreateBuffer()
{
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indices_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void AssimpMesh::Draw()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void AssimpMesh::ClearBuffer()
{
	glDeleteBuffers(1, &vertex_buffer);
	glDeleteBuffers(1, &uv_buffer);
	glDeleteBuffers(1, &normal_buffer);
}

AssimpRep::AssimpRep(const char * fp) :
	path(fp),
	meshes(std::vector<AssimpMesh*>())
{
}

AssimpRep::~AssimpRep()
{
	for (auto iter = meshes.begin(); iter != meshes.end(); iter++)
		delete *iter;

	meshes.clear();
}

void AssimpRep::LoadAssimp()
{
	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	if (nullptr == scene)
		return;

	meshes.reserve(scene->mNumMeshes);
	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		AssimpMesh* mesh = new AssimpMesh();
		mesh->BuildMesh(scene->mMeshes[i]);
		meshes.push_back(mesh);
	}
}

void AssimpRep::CreateBuffer()
{
	for (auto iter = meshes.begin(); iter != meshes.end(); iter++)
		(*iter)->CreateBuffer();
}

void AssimpRep::Draw()
{
	for (auto iter = meshes.begin(); iter != meshes.end(); iter++)
		(*iter)->Draw();
}

void AssimpRep::ClearBuffer()
{
	for (auto iter = meshes.begin(); iter != meshes.end(); iter++)
		(*iter)->ClearBuffer();
}
