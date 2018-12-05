#include "settings.h"
#include "picking_object.h"

Settings::Settings() :
	view_position(glm::vec3()),
	view_matrix(glm::mat4(1.0f)),
	projection_matrix(glm::mat4(1.0f)),
	window(nullptr),
	picking_object_manager(new PickingObject())
{}

Settings::~Settings()
{
	delete picking_object_manager;
	picking_object_manager = nullptr;
}
