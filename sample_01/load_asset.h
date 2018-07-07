#ifndef _LOAD_ASSET_H_
#define _LOAD_ASSET_H_

#include <GL\glew.h>
#include "assimp\vector3.h"

class AssimpRep
{
public:
	static GLuint LoadAsset(const char* path, int& num_ver);
	static void GetBound(const char* path, struct aiVector3t<float>* min, struct aiVector3t<float>* max);
};
#endif