#include "load_asset.h"

#include "assimp\scene.h"
#include "assimp\cimport.h"
#include "assimp\postprocess.h"
#include "assimp\matrix4x4.h"

#include <iostream>
#include <GL\glew.h>
#include <vector>

struct Vertex
{
	float x;
	float y;
	float z;

	Vertex(float _x, float _y, float _z) :
		x(_x),
		y(_y),
		z(_z)
	{}
};

struct Face
{
	Vertex v1;
	Vertex v2;
	Vertex v3;

	Face() :
		v1(0.0, 0.0, 0.0),
		v2(0.0, 0.0, 0.0),
		v3(0.0, 0.0, 0.0)
	{}
};

static std::vector<Face> GetFace(aiMesh* mesh)
{
	std::vector<Face> fs = std::vector<Face>();
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices / 3; j++)
		{
			Face f;
			Vertex v1(mesh->mVertices[mesh->mFaces[i].mIndices[3 * j]].x,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j]].y,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j]].z);

			Vertex v2(mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 1]].x,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 1]].y,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 1]].z);

			Vertex v3(mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 2]].x,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 2]].y,
				mesh->mVertices[mesh->mFaces[i].mIndices[3 * j + 2]].z);

			f.v1 = v1;
			f.v2 = v2;
			f.v3 = v3;

			fs.push_back(f);
		}
	}

	return fs;
}

static std::vector<Face> GetMesh(const aiScene* scene)
{
	std::vector<Face> fs = std::vector<Face>();
	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
	{
		std::vector<Face> sfs = GetFace(scene->mMeshes[i]);
		fs.insert(fs.end(), sfs.begin(), sfs.end());
	}

	return fs;
}

GLuint AssimpRep::LoadAsset(const char * path, int& num_ver)
{
	GLuint vertex_buffer;
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (nullptr != scene)
	{
		std::vector<Face> fs = GetMesh(scene);
		GLfloat* ver_buffer_data = new GLfloat[fs.size() * 9]();
		for (int i = 0; i < fs.size(); i++)
		{
			ver_buffer_data[i * 9] = fs.at(i).v1.x;
			ver_buffer_data[i * 9 + 1] = fs.at(i).v1.y;
			ver_buffer_data[i * 9 + 2] = fs.at(i).v1.z;
			ver_buffer_data[i * 9 + 3] = fs.at(i).v2.x;
			ver_buffer_data[i * 9 + 4] = fs.at(i).v2.y;
			ver_buffer_data[i * 9 + 5] = fs.at(i).v2.z;
			ver_buffer_data[i * 9 + 6] = fs.at(i).v3.x;
			ver_buffer_data[i * 9 + 7] = fs.at(i).v3.y;
			ver_buffer_data[i * 9 + 8] = fs.at(i).v3.z;
		}

		glBufferData(GL_ARRAY_BUFFER, fs.size() * 9, ver_buffer_data, GL_STATIC_DRAW);
		num_ver = fs.size() * 9;
		delete[] ver_buffer_data;
	}

	delete scene;

	return vertex_buffer;
}

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

static void get_bounding_box_for_node(
	const aiScene* scene,
	const struct aiNode* nd,
	aiVector3t<float>* min,
	aiVector3t<float>* max,
	aiMatrix4x4t<float>* trafo)
{
	aiMatrix4x4t<float> prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			aiVector3t<float> tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = aisgl_min(min->x, tmp.x);
			min->y = aisgl_min(min->y, tmp.y);
			min->z = aisgl_min(min->z, tmp.z);

			max->x = aisgl_max(max->x, tmp.x);
			max->y = aisgl_max(max->y, tmp.y);
			max->z = aisgl_max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(scene, nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

void AssimpRep::GetBound(const char * path, aiVector3t<float>* min, aiVector3t<float>* max)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	struct aiMatrix4x4t<float> trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene, scene->mRootNode, min, max, &trafo);
}
