#include "global_vars.h"
#include "draw_line_program.h"
#include "common/shader.hpp"

GlobalVars::GlobalVars() :
	debug_bullet_engine(new DrawLineProgram(LoadShaders("DrawLine.vertexshader", "DrawLine.fragmentshader")))
{
}

GlobalVars::~GlobalVars()
{
	delete debug_bullet_engine;
	debug_bullet_engine = nullptr;
}
