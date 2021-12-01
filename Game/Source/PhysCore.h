#pragma once
#include "RigidBody.h"
#include "List.h"
class PhysCore
{
public:
	PhysCore(fPoint gravity);

	~PhysCore();

	void Update(float simulationTime);

	bool CheckCollision(RigidBody* body);

	void AddRigidBody(RigidBody* body);

	void DeleteRigidBody(RigidBody* body);

	bool BoxCOlBox(RigidBody& b1, RigidBody& b2);

	bool CircleCOlCircle(RigidBody& b1, RigidBody& b2);

	bool BoxCOlCircle(RigidBody& b1, RigidBody& b2);

	void SetWind(fPoint windforce) 
	{
		wind = windforce;
	}

	void SetGravity(fPoint gravityforce)
	{
		gravity = gravityforce;
	}

	fPoint GetWind() 
	{
		return wind;
	}

	fPoint GetGravity()
	{
		return gravity;
	}

private:

	fPoint gravity;

	float density = 1.0f;

	fPoint wind = { 0,0 };

public:

	List<RigidBody*> rigidBodies;
};

