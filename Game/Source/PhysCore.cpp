#include "PhysCore.h"

PhysCore::PhysCore(fPoint gravity)
{
	this->gravity = gravity;
}

PhysCore::~PhysCore()
{
	rigidBodies.clearPtr();
}

void PhysCore::Update(float simulationTime)
{
	for (int i = 0; i < rigidBodies.count(); i++)
	{
		// Step #0 Reset acceleration and forces

		rigidBodies[i]->ResetForces();

		// Step #1 Calculate Forces (TotalForces = GravityForce + AdditionalForce)
		
		//	gravity
		rigidBodies[i]->AddForceToCenter(gravity * rigidBodies[i]->gravityScale * rigidBodies[i]->GetMass());

		//	Drag	(0.5 * density * relative velocity square * surface * Drag coeficient)
		fPoint dragForce;
		fPoint relativeVelocity;
		float relativeVelocityModule;
		float magnitudDrag;

		// Calcular velocidad relativa entre viento y body
		relativeVelocity.x = wind.x -rigidBodies[i]->GetLinearVelocity().x;
		relativeVelocity.y = wind.y -rigidBodies[i]->GetLinearVelocity().y;

		// Calcular el modulo de la velocidad relativa
		relativeVelocityModule = relativeVelocity.Module();

		// Calcular el magnitud del drag
		magnitudDrag = 0.5f * density * rigidBodies[i]->surface * pow(relativeVelocityModule,2) * rigidBodies[i]->GetDragCoheficient();
		
		fPoint nor = relativeVelocity.Normalize();

		// Calcular la fuerza de drag
		dragForce = nor * magnitudDrag;

		rigidBodies[i]->AddForceToCenter(dragForce);

		rigidBodies[i]->totalForce = rigidBodies[i]->additionalForce;
		rigidBodies[i]->additionalForce = { 0,0 };

		// Step #2 Calculate Newton's Second law (acceleration)
		rigidBodies[i]->acceleration = rigidBodies[i]->totalForce / rigidBodies[i]->mass;

		// Step #3 Integrate with Verlet
		rigidBodies[i]->position += rigidBodies[i]->velocity * simulationTime + rigidBodies[i]->acceleration * (simulationTime * simulationTime * 0.5f);
		rigidBodies[i]->velocity += rigidBodies[i]->acceleration * simulationTime;

		// Step #4: solve collisions
		// CheckCollisions()
	}
}

bool PhysCore::CheckCollision(RigidBody* body)
{
	//Check if body is colliding with any other body on rigidBodies

	// Collision Circle && Rect
	//https://www.cnblogs.com/shadow-lr/p/BoxCircleIntersect.html
	return true;
}

void PhysCore::AddRigidBody(RigidBody* body)
{
	rigidBodies.add(body);
}

void PhysCore::DeleteRigidBody(RigidBody* body)
{
	rigidBodies.del(rigidBodies.At(rigidBodies.find(body)));
}

bool PhysCore::BoxCOlBox(RigidBody& b1, RigidBody& b2)
{
	//if (b1.shape != RECT || b2.shape != RECT) return false;

	//if(b1.position.x<b2.position.x+b2)

	return false;
}

bool PhysCore::CircleCOlCircle(RigidBody& b1, RigidBody& b2)
{
	return false;
}

bool PhysCore::BoxCOlCircle(RigidBody& b1, RigidBody& b2)
{
	return false;
}
