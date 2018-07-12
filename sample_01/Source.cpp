#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "window_wrap.h"
#include "shader.h"
#include "load_asset.h"
#include "view_port.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>


using namespace glm;

GLuint loadDDS(const char * imagepath);

static const float HEIGHT_SCREEN = 768.0f;
static const float WIDTH_SCREEN = 1366.0f;

//#define USED_TEXTURE
int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW \n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWWindowWrap glfw_wrap(glfwCreateWindow(1366, 768, "Sample 01", nullptr, nullptr));
	if (nullptr == glfw_wrap.glfw_window)
	{
		fprintf(stderr, "Failed to open GLFW window \n");
		getchar();
		return -1;
	}

	glfwMakeContextCurrent(glfw_wrap.glfw_window);

	if (GLEW_OK != glewInit())
	{
		fprintf(stderr, "Failed to initialize GLEW \n");
		getchar();
		return -1;
	}

	glfwSetInputMode(glfw_wrap.glfw_window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(glfw_wrap.glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//setup camera
	GLuint program_id = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	GLuint light_id = glGetUniformLocation(program_id, "LightPosition_worldspace");
	GLuint matrix_id = glGetUniformLocation(program_id, "MVP");
	GLuint view_matrix_id = glGetUniformLocation(program_id, "V");
	GLuint model_matrix_id = glGetUniformLocation(program_id, "M");

	//create objects
	AssimpRep* assimp_rep = new AssimpRep("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\model_test\\box.3DS");
	assimp_rep->LoadAssimp();

	//second object
	AssimpRep* obj_ref = new AssimpRep("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\model_test\\box.3DS");
	obj_ref->LoadAssimp();
	obj_ref->MoveModel(8.0f, 8.0f, 8.0f);

	//texture
	GLuint texture = loadDDS("D:\\Team TP HCM\\Game Dev\\openGL\\sample3D\\Monkey_head\\uvmap.DDS");
	GLuint texture_id = glGetUniformLocation(program_id, "myTextureSampler");

	//set up light
	glm::vec3 light_pos = glm::vec3(8.0f, 8.0f, 8.0f);
	glUniform3f(light_id, light_pos.x, light_pos.y, light_pos.z);

	//set up camera
	Point min = Point(0.0, 0.0, 0.0);
	Point max = Point(0.0, 0.0, 0.0);
	assimp_rep->GetBoundModel(min, max);

	Point dmin = Point(0.0, 0.0, 0.0);
	Point dmax = Point(0.0, 0.0, 0.0);
	obj_ref->GetBoundModel(dmin, dmax);

	if (dmin.x < min.x)
		min.x = dmin.x;
	if (dmin.y < min.y)
		min.y = dmin.y;
	if (dmin.z < min.z)
		min.z = dmin.z;

	if (dmax.x > max.x)
		max.x = dmax.x;
	if (dmax.y > max.y)
		max.y = dmax.y;
	if (dmax.z > max.z)
		max.z = dmax.z;

	ViewPort vp = ViewPort(glfw_wrap.glfw_window);
	vp.SetupCenterPnt(0.5 * (min.x + max.x), 0.5 * (min.y + max.y), 0.5 * (min.z + max.z));
	vp.SetupCoordinate(min.Distance(max) * std::sqrt(3.0), 0.0, 0.0);
	vp.SetupMouseCoor();

	//Assimp load buffer
	assimp_rep->CreateBuffer();
	obj_ref->CreateBuffer();

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//use shader
		glUseProgram(program_id);
		vp.MouseInput();
		glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &vp.mvp[0][0]);
		glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &vp.view_matrix[0][0]);
		glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &vp.model_matrix[0][0]);

#ifdef USED_TEXTURE
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(TextureID, 0);
#endif
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glUniform1i(texture_id, 0);

		//draw here
		assimp_rep->Draw();
		obj_ref->Draw();
		//end draw here

		glfwSwapBuffers(glfw_wrap.glfw_window);
		glfwPollEvents();
	} while (glfwGetKey(glfw_wrap.glfw_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		0 == glfwWindowShouldClose(glfw_wrap.glfw_window));

	glDeleteProgram(program_id);
	assimp_rep->ClearBuffer();
	obj_ref->ClearBuffer();

	delete assimp_rep;
	delete obj_ref;

	return 0;
}

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint loadDDS(const char * imagepath)
{
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;
}
