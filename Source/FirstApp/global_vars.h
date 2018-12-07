#ifndef _GLOBAL_VARS_H_
#define _GLOBAL_VARS_H_

class DrawLineProgram;
class CameraEntity;
class GlobalVars
{
public:
	DrawLineProgram* debug_bullet_engine;
	bool is_mouse_pressed;
	CameraEntity* camera_entity;

	GlobalVars();
	~GlobalVars();
};

#endif