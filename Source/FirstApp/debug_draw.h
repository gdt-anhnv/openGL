#ifndef _DEBUG_DRAW_H_
#define _DEBUG_DRAW_H_

#include "LinearMath/btIDebugDraw.h"

class MyDebugDraw : public btIDebugDraw
{
	int debug_mode;

public:
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int getDebugMode() const;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
};

#endif