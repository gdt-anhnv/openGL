#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <gl/glew.h>

class Entity
{
private:
	float position[3];
public:
	Entity();
	~Entity();

	void SetPosition(float x, float y, float z);
	GLuint GetVertexBuffer();
	GLuint GetColorBuffer();
};

#endif