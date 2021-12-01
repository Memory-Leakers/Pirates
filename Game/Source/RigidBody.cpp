#include "RigidBody.h"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(fPoint pos, RigidBodyType type,float width,float height)
{
	this->position = pos;
	this->type = type;
	this->shape = ShapeType::RECT;
	this->width = width;
	this->height = height;
}

RigidBody::RigidBody(fPoint pos, RigidBodyType type, float radius)
{
	this->position = pos;
	this->type = type;
	this->shape = ShapeType::CIRCLE;
	this->radius = radius;
}

RigidBody::RigidBody(RigidBody& copy)
{
	this->position = copy.position;
	this->acceleration = copy.acceleration;
	this->velocity = copy.velocity;
	this->friction = copy.friction;
	this->drag = copy.drag;
	this->mass = copy.mass;
	this->restitution = copy.restitution;
	this->rotation = copy.rotation;
	this->type = copy.type;
	this->width = copy.width;
	this->height = copy.height;
	this->radius = copy.radius;
	this->shape = copy.shape;
	this->gravityScale = copy.gravityScale;
}

void RigidBody::AddForceToCenter(fPoint force)
{
	additionalForce += force;
}

void RigidBody::ResetForces()
{
	acceleration = totalForce = { 0,0 };
}
