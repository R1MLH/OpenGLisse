#pragma once

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include <unordered_map>

class myObject;

class myPhysics
{
public:
	enum body_type { CONVEX, CONCAVE };
	std::unordered_multimap<const myObject *, btCollisionObject *> obj_physics;

	double last_time;

	btDynamicsWorld* world;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* broadphase;
	btConstraintSolver* solver;


	myPhysics();
	~myPhysics();

	void addObject(myObject *, body_type = myPhysics::CONVEX, btCollisionObject::CollisionFlags f = btCollisionObject::CF_STATIC_OBJECT, float mass = 0.0f, float restitution = 0.0f);

	void getModelMatrix(myObject *);
	void setModelMatrix(myObject *);

	void stepSimulation(double);
	void setTime(double);

	btCollisionObject * & operator[](const myObject *);
};

