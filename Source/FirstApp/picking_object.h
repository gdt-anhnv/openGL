#ifndef _PICKING_OBJECT_H_
#define _PICKING_OBJECT_H_

template <typename T> class btAlignedObjectArray;
class btCollisionShape;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btConstraintSolver;
class btDefaultCollisionConfiguration;
class btDiscreteDynamicsWorld;

class PickingObject
{
public:
	btAlignedObjectArray<btCollisionShape*>* collision_shapes;
	btBroadphaseInterface* broadphase;
	btCollisionDispatcher* dispatcher;
	btConstraintSolver* solver;
	btDefaultCollisionConfiguration* collision_configuration;
	btDiscreteDynamicsWorld* dynamic_world;

public:
	PickingObject();
	~PickingObject();
};

#endif