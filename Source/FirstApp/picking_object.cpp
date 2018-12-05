#include "picking_object.h"

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

PickingObject::PickingObject() :
	collision_shapes(nullptr),
	broadphase(nullptr),
	dispatcher(nullptr),
	solver(nullptr),
	collision_configuration(nullptr),
	dynamic_world(nullptr)
{
	collision_shapes = new btAlignedObjectArray<btCollisionShape*>();
	broadphase = new btDbvtBroadphase();
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration);
	solver = new btSequentialImpulseConstraintSolver();

	dynamic_world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
}

PickingObject::~PickingObject()
{
	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamic_world->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamic_world->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamic_world->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collision_shapes->size(); j++)
	{
		btCollisionShape* shape = (*collision_shapes)[j];
		(*collision_shapes)[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamic_world;
	dynamic_world = nullptr;

	//delete solver
	delete solver;
	solver = nullptr;

	//delete broadphase
	delete broadphase;
	broadphase = nullptr;

	//delete dispatcher
	delete dispatcher;
	dispatcher = nullptr;

	delete collision_configuration;
	collision_configuration = nullptr;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collision_shapes->clear();
	delete collision_shapes;
	collision_shapes = nullptr;
}
