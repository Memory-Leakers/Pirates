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

	void BoxColBox(RigidBody& b1, RigidBody& b2);

	void CircleColCircle(RigidBody& b1, RigidBody& b2);

	void BoxColCircle(RigidBody& b1, RigidBody& b2);

	void ResolveColForce(RigidBody& b1, RigidBody& b2, fPoint colPoint);

	float submergedVolume(RigidBody* body, RigidBody* water);

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

	// Detect collision point in 2 shapes
	fPoint CollisionPoint(RigidBody& b1, RigidBody& b2);

	// Dectet center of shape -> collision point(any point) vector
	fPoint CollisionDir(RigidBody& b1, fPoint colPoint);

	// Resolve clamping case
	void ResolveClipping(RigidBody& b1, RigidBody& b2);

	/// <summary>
	/// Check intersectionPoint of 2 lines
	/// </summary>
	/// <param name="p1">init of line 1</param>
	/// <param name="p2">end of line 1</param>
	/// <param name="p3">init of line 2</param>
	/// <param name="p4">end of line 2</param>
	/// <returns></returns>
	fPoint IntersectionPoint(fPoint p1, fPoint p2, fPoint p3, fPoint p4);
private:

	fPoint gravity;

	float density = 1.0f;

	fPoint wind = { 0,0 };

	int resolveColForce = 5;

public:

	bool gravityOn = true;
	bool buoyancyOn = true;
	bool hydrioDragOn = true;
	bool aeroDragOn = true;
	bool clippingOn = true;
	bool frictioOn = true;

	List<RigidBody*> rigidBodies;
};