#include "entity.h"
#include "basic_program.h"
#include "draw_line_program.h"
#include "settings.h"
#include "DataStructures/singleton.h"
#include "picking_object.h"
#include "global_vars.h"
#include "camera_entity.h"

// Include GLEW
#include "GL/glew.h"

// Include GLFW
#include "GLFW/glfw3.h"

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"

//bullet engine
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

// Include standard headers
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>


void MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	return;
}

int main(void)
{
	Singleton<Settings>::AssignInstance(new Settings());

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	Singleton<Settings>::GetInstance()->window = glfwCreateWindow(1024, 768, "OpenGL", NULL, NULL);
	if (NULL == Singleton<Settings>::GetInstance()->window) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Singleton<Settings>::GetInstance()->window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwSetMouseButtonCallback(Singleton<Settings>::GetInstance()->window, MouseEvent);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(Singleton<Settings>::GetInstance()->window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Create and compile our GLSL program from the shaders

	std::list<Entity*> ents = std::list<Entity*>();

	{
		Entity* entity = new Entity();
		entity->SetPosition(2.0f, 0.0f, 0.0f);
		ents.push_back(entity);
	}

	{
		Entity* entity = new Entity();
		entity->SetPosition(-2.0f, 0.0f, 0.0f);
		ents.push_back(entity);
	}

	{
		Entity* entity = new Entity();
		entity->SetPosition(0.0f, 3.0f, 0.0f);
		ents.push_back(entity);
	}

	std::list<EntBuffer> vertexbuffers;
	glm::vec3 min = glm::vec3();
	glm::vec3 max = glm::vec3();
	ents.front()->GetExtremePoints(min, max);

	for (auto iter = ents.begin(); iter != ents.end(); iter++)
	{
		auto vb = (*iter)->GetVertexBuffer();
		auto tb = (*iter)->GetTextureBuffer();
		auto nb = (*iter)->GetNormalBuffer();
		auto sb = (*iter)->GetDataSize();
		for (int i = 0; i < vb.size(); i++)
		{
			vertexbuffers.push_back(EntBuffer(vb[i], tb[i], nb[i], sb[i]));
		}

		glm::vec3 sub_min = glm::vec3();
		glm::vec3 sub_max = glm::vec3();
		(*iter)->GetExtremePoints(sub_min, sub_max);

		if (min.x > sub_min.x)
			min.x = sub_min.x;
		if (min.y > sub_min.y)
			min.y = sub_min.y;
		if (min.z > sub_min.z)
			min.z = sub_min.z;
		if (max.x < sub_max.x)
			max.x = sub_max.x;
		if (max.y < sub_max.y)
			max.y = sub_max.y;
		if (max.z < sub_max.z)
			max.z = sub_max.z;

		btCollisionShape* shape = new btBoxShape(
			btVector3(
				0.5 * (sub_max.x - sub_min.x),
				0.5 * (sub_max.y - sub_min.y),
				0.5 * (sub_max.z - sub_min.z)));
		btTransform trans = btTransform();

		trans.setIdentity();
		trans.setOrigin(btVector3(
			0.5 * (sub_min.x + sub_max.x),
			0.5 * (sub_min.y + sub_max.y),
			0.5 * (sub_min.z + sub_max.z)));
		Singleton<Settings>::GetInstance()->picking_object_manager
			->collision_shapes->push_back(shape);
		btDefaultMotionState* motion_state = new btDefaultMotionState(trans);
		btScalar mass(0.);
		btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, shape);

		btRigidBody* rb = new btRigidBody(rb_info);
		Singleton<Settings>::GetInstance()->picking_object_manager
			->dynamic_world->addRigidBody(rb);
		static int index = 10;
		rb->setUserIndex(index++);
	}

	double length = glm::length(max - min);
	glm::vec3 center = glm::vec3(
		0.5 * (max.x + min.x),
		0.5 * (max.y + min.y),
		0.5 * (max.z + min.z));

	Singleton<Settings>::GetInstance()->projection_matrix =
		glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	Singleton<GlobalVars>::AssignInstance(new GlobalVars());
	CameraEntity* cam_ent = Singleton<GlobalVars>::GetInstance()->camera_entity;
		glfwGetCursorPos(Singleton<Settings>::GetInstance()->window,
			&cam_ent->mouse_position[0], &cam_ent->mouse_position[1]);

	cam_ent->SetPosition(0.0f, 0.0f, 18.0f);
	cam_ent->SetViewAt(0.0f, 0.0f, 0.0f);
	float up_vec[3] = { 0.0, 1.0, 0.0 };
	cam_ent->InitOrientation(up_vec);
	Singleton<Settings>::GetInstance()->view_matrix = cam_ent->GetViewMatrix();

	BasicProgram* bp = new BasicProgram(LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader"));
	bp->SetLightPosition(400.0f, 400.0f, 400.0f);

	for (auto iter = vertexbuffers.begin(); iter != vertexbuffers.end(); iter++)
	{
		bp->AddEntity(*iter);
		bp->PreDrawing();
	}
	
	Singleton<GlobalVars>::GetInstance()->debug_bullet_engine->PreDrawing();
	Singleton<Settings>::GetInstance()->picking_object_manager->dynamic_world->debugDrawWorld();
	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Singleton<Settings>::GetInstance()->view_matrix = cam_ent->GetViewMatrix();
		cam_ent->RotateCam();

		bp->Draw();
		Singleton<GlobalVars>::GetInstance()->debug_bullet_engine->Draw();

		// Swap buffers
		glfwSwapBuffers(Singleton<Settings>::GetInstance()->window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(Singleton<Settings>::GetInstance()->window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		0 == glfwWindowShouldClose(Singleton<Settings>::GetInstance()->window));

	bp->PostDrawing();
	Singleton<GlobalVars>::GetInstance()->debug_bullet_engine->PostDrawing();

	delete bp;

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	for (auto iter = ents.begin(); iter != ents.end(); iter++)
		delete *iter;

	ents.clear();

	Singleton<Settings>::destroy();
	Singleton<GlobalVars>::destroy();

	return 0;
}

