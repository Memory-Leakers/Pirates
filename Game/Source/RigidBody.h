#pragma once
#include "Point.h"
#include "List.h"

enum RigidBodyType
{
	STATIC,
	DYNAMIC,
	KINEMATIC,
	WATER,
};

enum class ShapeType
{
	CIRCLE,
	RECT,
};


class GameObject;

class RigidBody
{
private:
	// Position
	fPoint position = { 0.0, 0.0 };

	//Properties
	float restitution = 0.0f;
	float friction = 0.0f;
	float coefficientDrag = 0.0f;
	float hydrodynamicDrag = 0.3f;
	float mass = 1.0f;
	fPoint velocity = { 0.0f, 0.0f };
	fPoint acceleration = { 0.0, 0.0 };
	float rotation = 0.0f;
	float gravityScale = 1.0f;
	float surface = 1.0f;
	float width = 1.0f;
	float height = 1.0f;
	float radius = 1.0f;

	RigidBodyType type = STATIC;
	ShapeType shape = ShapeType::RECT;

	float maximumVelocity = 1000.0f;

	fPoint totalForce = { 0.0, 0.0 };
	fPoint additionalForce = { 0.0, 0.0 };

	List<RigidBody*> collisionList;

	iPoint ColDir = { 0, 0 };

	GameObject* gObj;

public:
	RigidBody();

	~RigidBody();

	RigidBody(iPoint pos, RigidBodyType type, float width, float height, GameObject* gObj = nullptr);

	RigidBody(iPoint pos, RigidBodyType type, float radius, GameObject* gObj = nullptr);

	RigidBody(RigidBody& copy);

	void OnCollisionEnter(RigidBody* col);

	void OnCollisionStay(RigidBody* col);

	void OnCollisionExit(RigidBody* col);

	void AddForceToCenter(fPoint force);

	iPoint GetPosition()
	{
		int x = (int)METERS_TO_PIXELS(position.x);
		int y = (int)METERS_TO_PIXELS(position.y);

		iPoint pos = { (int)(METERS_TO_PIXELS(position.x)), (int)(METERS_TO_PIXELS(position.y)) };
		return pos;
	}
	void SetPosition(iPoint pos)
	{
		this->position.x = PIXELS_TO_METERS(pos.x);
		this->position.y = PIXELS_TO_METERS(pos.y);
	}
	float GetRestitution()
	{
		return restitution;
	}
	void SetRestitution(float res)
	{
		this->restitution = res;
	}

	float GetFriction()
	{
		return friction;
	}
	void SetFriction(float f)
	{
		this->friction = f;
	}

	float GetMass()
	{
		return mass;
	}
	void SetMass(float mass)
	{
		this->mass = mass;
	}

	float GetDragCoeficient()
	{
		return coefficientDrag;
	}
	void SetDragCoeficient(float drag)
	{
		this->coefficientDrag = drag;
	}

	float GetHydrodynamicDragCoeficient()
	{
		return hydrodynamicDrag;
	}
	void SetHydrodynamicDragCoeficient(float hydrodrag)
	{
		this->hydrodynamicDrag = hydrodrag;
	}

	fPoint GetLinearVelocity()
	{
		return velocity;
	}
	void SetLinearVelocity(fPoint velocity)
	{
		velocity.x > maximumVelocity ? maximumVelocity : velocity.x < -maximumVelocity ? -maximumVelocity : velocity.x;
		velocity.y > maximumVelocity ? maximumVelocity : velocity.y < -maximumVelocity ? -maximumVelocity : velocity.y;

		this->velocity = velocity;
	}

	void SetGravityScale(float gravityScale)
	{
		this->gravityScale = gravityScale;
	}

	float GetGravityScale()
	{
		return gravityScale;
	}

	float GetRotation()
	{
		return rotation;
	}
	void SetRotation(float rotation)
	{
		this->rotation = rotation;
	}
	float GetRadius()
	{
		return radius;
	}
	RigidBodyType GetType()
	{
		return type;
	}

	bool Contains(fPoint pos)
	{
		if (shape == ShapeType::RECT)
		{
			if (pos.x >= position.x - width / 2 && pos.x <= position.x + width / 2 &&
				pos.y >= position.y - height / 2 && pos.y <= position.y + height / 2)
			{
				return true;
			}
		}
		else
		{
			if (sqrt(pow(position.x - pos.x, 2) + pow(position.y - pos.y, 2)) <= radius)
			{
				return true;
			}
		}

		return false;
	}

private:

	void ResetForces();

	friend class PhysCore;
};