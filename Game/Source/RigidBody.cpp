#include "RigidBody.h"
#include "GameObject.h"

RigidBody::RigidBody()
{

}

RigidBody::~RigidBody()
{
}

RigidBody::RigidBody(iPoint pos, RigidBodyType type, float width, float height, GameObject* gObj)
{
	this->position.x = PIXELS_TO_METERS(pos.x);
	this->position.y = PIXELS_TO_METERS(pos.y);
	this->type = type;
	this->shape = ShapeType::RECT;
	this->width = PIXELS_TO_METERS(width);
	this->height = PIXELS_TO_METERS(height);
	this->gObj = gObj;
}

RigidBody::RigidBody(iPoint pos, RigidBodyType type, float radius, GameObject* gObj)
{
	this->position.x = PIXELS_TO_METERS(pos.x);
	this->position.y = PIXELS_TO_METERS(pos.y);
	this->type = type;
	this->shape = ShapeType::CIRCLE;
	this->radius = PIXELS_TO_METERS(radius);
	this->gObj = gObj;
}

RigidBody::RigidBody(RigidBody& copy)
{
	this->position = copy.position;
	this->acceleration = copy.acceleration;
	this->velocity = copy.velocity;
	this->friction = copy.friction;
	this->coefficientDrag = copy.coefficientDrag;
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

void RigidBody::OnCollisionEnter(RigidBody* col)
{
	printf("Col enter\n");

	if (gObj != nullptr)
	{
		gObj->OnCollisionEnter(col);
	}
}

void RigidBody::OnCollisionStay(RigidBody* col)
{
	//printf("Col stay\n");
}

void RigidBody::OnCollisionExit(RigidBody* col)
{
	//printf("Col exit\n");
	if (gObj != nullptr)
	{
		gObj->OnCollisionExit(col);
	}
}

void RigidBody::AddForceToCenter(fPoint force)
{
	additionalForce += force;
}

void RigidBody::ResetForces()
{
	acceleration = totalForce = { 0,0 };
}
