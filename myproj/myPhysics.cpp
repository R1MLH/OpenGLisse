#include "myPhysics.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "myObject.h"
#include <iostream>

myPhysics::myPhysics()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
	world->setGravity(btVector3(0, -10, 0));
}


myPhysics::~myPhysics()
{
	delete world;
	delete solver;
	delete broadphase;
	delete dispatcher;
	delete collisionConfig;
}

void myPhysics::addObject(myObject *obj, body_type objtype, btCollisionObject::CollisionFlags f, float mass, float restitution)
{
	if (objtype == myPhysics::CONVEX)
	{
		btConvexHullShape* shape = new btConvexHullShape(&obj->vertices[0][0], static_cast<size_t>(obj->vertices.size()), sizeof(btScalar) * 3);
		shape->optimizeConvexHull();
		shape->initializePolyhedralFeatures();
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(obj->model_matrix));

		btVector3 localInertia(0, 0, 0);
		if (mass != 0.0f) shape->calculateLocalInertia(mass, localInertia);
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
		btRigidBody* body = new btRigidBody(cInfo);
		body->forceActivationState(DISABLE_DEACTIVATION);

		body->setRestitution(restitution);
		body->setCollisionFlags(f);

		obj_physics.emplace(obj, body);
		world->addRigidBody(body);
	}

	if (objtype == myPhysics::CONCAVE)
	{
		btTriangleIndexVertexArray* triangle_index_vertex_array = new btTriangleIndexVertexArray(obj->indices.size(), &obj->indices[0][0], 3 * sizeof(int), obj->vertices.size(), &obj->vertices[0][0], 3 * sizeof(btScalar));

		btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(triangle_index_vertex_array, true);
		btTransform startTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(obj->model_matrix));

		btCollisionObject* body = new btCollisionObject();
		body->setWorldTransform(startTransform);
		body->setCollisionShape(shape);
	
		body->setRestitution(restitution);
		body->setCollisionFlags(f);

		body->forceActivationState(DISABLE_DEACTIVATION);

		obj_physics.emplace(obj, body);
		world->addCollisionObject(body);
	}
}

void myPhysics::getModelMatrix(myObject *obj)
{
	btTransform t;

	btRigidBody *b = dynamic_cast<btRigidBody*>(obj_physics.find(obj)->second);

	if (b == nullptr)
		t = (obj_physics.find(obj)->second)->getWorldTransform();
	else
		b->getMotionState()->getWorldTransform(t);

	t.getOpenGLMatrix(glm::value_ptr(obj->model_matrix));
}

void myPhysics::setModelMatrix(myObject *obj)
{
	//Not checked completely for correctness. Use with care.
	btTransform t;
	t.setFromOpenGLMatrix(glm::value_ptr(obj->model_matrix));

	btCollisionObject *b = obj_physics.find(obj)->second;
	if (b != nullptr)
		b->setWorldTransform(t);
}

void myPhysics::stepSimulation(double curr_time)
{
	world->stepSimulation( (curr_time - last_time)/1.0f );
	last_time = curr_time;
}

void myPhysics::setTime(double curr_time)
{
	last_time = curr_time;
}

btCollisionObject* & myPhysics::operator[](const myObject *obj)
{
	return obj_physics.find(obj)->second;
}