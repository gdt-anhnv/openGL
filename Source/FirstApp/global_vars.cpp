#include "global_vars.h"
#include "draw_line_program.h"
#include "common/shader.hpp"
#include "camera_entity.h"

GlobalVars::GlobalVars() :
	debug_bullet_engine(new DrawLineProgram(LoadShaders("DrawLine.vertexshader", "DrawLine.fragmentshader"))),
	is_mouse_pressed(false),
	camera_entity(new CameraEntity())
{
}

GlobalVars::~GlobalVars()
{
	delete debug_bullet_engine;
	debug_bullet_engine = nullptr;

	delete camera_entity;
	camera_entity = nullptr;
}
