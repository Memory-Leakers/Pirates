#include "PhysCore.h"
#include <algorithm>

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
		if (rigidBodies[i]->type == RigidBodyType::STATIC || rigidBodies[i]->type == RigidBodyType::WATER || rigidBodies[i] == nullptr) continue;

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
		relativeVelocity.x = wind.x - rigidBodies[i]->GetLinearVelocity().x;
		relativeVelocity.y = wind.y - rigidBodies[i]->GetLinearVelocity().y;

		// Calcular el modulo de la velocidad relativa
		relativeVelocityModule = relativeVelocity.Module();

		// Calcular el magnitud del drag
		magnitudDrag = 0.5f * density * rigidBodies[i]->surface * pow(relativeVelocityModule, 2) * rigidBodies[i]->GetDragCoeficient();

		fPoint nor = relativeVelocity.Normalize();

		// Calcular la fuerza de drag
		dragForce = nor * magnitudDrag;

		rigidBodies[i]->AddForceToCenter(dragForce);

		rigidBodies[i]->totalForce = rigidBodies[i]->additionalForce;
		rigidBodies[i]->additionalForce = { 0,0 };

		// Step #2 Calculate Newton's Second law (acceleration)
		rigidBodies[i]->acceleration = rigidBodies[i]->totalForce / rigidBodies[i]->mass;

		// Chek if collision Stay
		for (int j = 0; j < rigidBodies[i]->collisionList.count(); j++)
		{
			if (rigidBodies[i]->collisionList[j]->type == RigidBodyType::WATER)
			{
				//Buoyancy (Density * gravity * area of the object flooded)
				fPoint buoyancyForce;
				float magnitudbuoyancy;
				fPoint direct = { 0, -10 };
				fPoint direction = direct.Normalize();

				float mod = gravity.Module();

				// 1 = 100%
				float submerge = submergedVolume(rigidBodies[i], rigidBodies[i]->collisionList[j]);

				// *2 = 200% para que pueda subir
				magnitudbuoyancy = density * mod * submerge * 2 * rigidBodies[i]->GetGravityScale();

				buoyancyForce = direction * magnitudbuoyancy;

				rigidBodies[i]->AddForceToCenter(buoyancyForce);

				// Calcular la fuerza de drag
				dragForce = (rigidBodies[i]->velocity * -1) * rigidBodies[i]->hydrodynamicDrag;

				rigidBodies[i]->AddForceToCenter(dragForce);
			}
			else
			{
				fPoint colPoint = CollisionPoint(*rigidBodies[i], *rigidBodies[i]->collisionList[j]);
				ResolveColForce(*rigidBodies[i], *rigidBodies[i]->collisionList[j], colPoint);
			}
		}

		// Check if next position is colling
		fPoint nextPosition = rigidBodies[i]->position + rigidBodies[i]->velocity * simulationTime + rigidBodies[i]->acceleration * (simulationTime * simulationTime * 0.5f);

		// Step #3 Integrate with Verlet
		rigidBodies[i]->position += rigidBodies[i]->velocity * simulationTime + rigidBodies[i]->acceleration * (simulationTime * simulationTime * 0.5f);
		rigidBodies[i]->velocity += rigidBodies[i]->acceleration * simulationTime;
	}

	// Despues de mover todos los objetos comparan la colision.
	for (int i = 0; i < rigidBodies.count(); i++)
	{
		if (rigidBodies[i]->type == RigidBodyType::DYNAMIC)
		{
			// Step #4: solve collisions
			CheckCollision(rigidBodies[i]);
		}
	}
}

bool PhysCore::CheckCollision(RigidBody* body)
{
	// Check if body is colliding with any other body on rigidBodies
	for (int i = 0; i < rigidBodies.count(); i++)
	{
		// If not is self
		if (i != rigidBodies.find(body))
		{
			if (body->shape == ShapeType::RECT && rigidBodies[i]->shape == ShapeType::RECT)
			{
				BoxColBox(*body, *rigidBodies[i]);
			}
			else if (body->shape == ShapeType::CIRCLE && rigidBodies[i]->shape == ShapeType::CIRCLE)
			{
				CircleColCircle(*body, *rigidBodies[i]);
			}
			else // CIRCLE col RECT || RECT col CIRCLE
			{
				BoxColCircle(*body, *rigidBodies[i]);
			}
		}
	}
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

COL_TYPE PhysCore::BoxColBox(RigidBody& b1, RigidBody& b2, bool trigger)
{
	// No collision case
	if (b1.position.x > b2.position.x + b2.width ||
		b1.position.x + b1.width < b2.position.x ||
		b1.position.y > b2.position.y + b2.height ||
		b1.position.y + b1.height < b2.position.y)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			// Collision Exit
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				b1.OnCollisionExit(&b2);
				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
				;
			}
		}
		return NONE;
	}

	// Collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		if (trigger)
		{
			return TRIGGER;
		}
		// Collision stay
		if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
		{
			b1.OnCollisionStay(&b2);
			ResolveColForce(b1, b2, CollisionPoint(b1, b2));
			return COLLISION;
		}
	}

	// Collision Enter
	b1.collisionList.add(&b2);
	b1.OnCollisionEnter(&b2);
	ResolveColForce(b1, b2, CollisionPoint(b1, b2));
	return COLLISION;
}

COL_TYPE PhysCore::CircleColCircle(RigidBody& b1, RigidBody& b2, bool trigger)
{
	float distX = b1.position.x - b2.position.x;
	float distY = b1.position.y - b2.position.y;
	float distance = sqrt(pow(distX, 2) + pow(distY, 2));

	//Collision
	if (distance <= b1.radius + b2.radius)
	{
		if (trigger)
		{
			return TRIGGER;
		}
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				b1.OnCollisionStay(&b2);
				return COLLISION;
			}
		}

		b1.collisionList.add(&b2);
		b1.OnCollisionEnter(&b2);

		fPoint colPoint = CollisionPoint(b1, b2);

		ResolveColForce(b1, b2, colPoint);
		return COLLISION;
	}

	//No Collision
	if (distance > b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				b1.OnCollisionExit(&b2);
				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
			}
		}
	}
}

COL_TYPE PhysCore::BoxColCircle(RigidBody& b1, RigidBody& b2, bool trigger)
{
	RigidBody* circ;
	RigidBody* rect;
	fPoint colPoint;

	float height;
	float width;
	float distance;

	if (b1.shape == ShapeType::RECT)
	{
		rect = &b1;
		circ = &b2;
	}
	else
	{
		rect = &b2;
		circ = &b1;
	}

	colPoint = CollisionPoint(*circ, *rect);

	distance = (colPoint - circ->position).magnitude();

	// Collision case
	if (distance <= circ->GetRadius())
	{
		if (trigger)
		{
			return TRIGGER;
		}

		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				b1.OnCollisionStay(&b2);
				ResolveColForce(b1, b2, colPoint);
				return COLLISION;
			}
		}
		b1.collisionList.add(&b2);
		b1.OnCollisionEnter(&b2);
		ResolveColForce(b1, b2, colPoint);
		return COLLISION;
	}

	// Cliping case

	// No collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
		{
			b1.OnCollisionExit(&b2);
			b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
		}
	}

	return NONE;
}

/// <summary>
/// Just resolve Dynamic body && Static body
/// </summary>
/// <param name="b1"></param>
/// <param name="b2"></param>
/// <param name="colPoint"></param>
void PhysCore::ResolveColForce(RigidBody& b1, RigidBody& b2, fPoint colPoint)
{
	RigidBody* dinBody;
	RigidBody* staticBody;

	if (b1.type == RigidBodyType::DYNAMIC && b2.type == RigidBodyType::STATIC)
	{
		dinBody = &b1;
		staticBody = &b2;
	}
	else if (b2.type == RigidBodyType::DYNAMIC && b1.type == RigidBodyType::STATIC)
	{
		dinBody = &b2;
		staticBody = &b1;
	}
	else if (b2.type == RigidBodyType::WATER || b1.type == RigidBodyType::WATER)
	{
		dinBody = &b1;
		staticBody = &b2;
		return;
	}
	else
	{
		printf("Canno't resolve collision force!");
		return;
	}

	fPoint b1Vel = dinBody->GetLinearVelocity();

	fPoint colCondition;

	// init Col Point codition
	if (b1.shape == b2.shape)
	{
		if (b1.shape == ShapeType::CIRCLE)
		{
			// circle && circle
			//colCondition = colPoint;

			colCondition = CollisionDir(*dinBody, colPoint);

			fPoint direction = CollisionDir(*dinBody, colPoint);
			float velMagnitud = b1Vel.magnitude();
			b1Vel = direction * velMagnitud * b1.restitution;
		}

		if (b1.shape == ShapeType::RECT)
		{
			printf("Can not resolve force rect & rect");
			return;
		}
	}
	else
	{
		// circle && rect
		colCondition = CollisionDir(*dinBody, colPoint);

		if (abs(colCondition.x) > abs(colCondition.y))
		{
			// colision en eje x
			if (colCondition.x > 0 && b1Vel.x <= 0 || colCondition.x < 0 && b1Vel.x >= 0)
			{
				//if (b1Vel.x == 0)b1Vel.x = resolveColForce * colCondition.x;
				b1Vel.x *= -dinBody->restitution;
			}
		}
		else
		{
			if (colCondition.y < 0 && b1Vel.y >= 0 || colCondition.y > 0 && b1Vel.y <= 0)
			{
				//if (b1Vel.y == 0)b1Vel.y = -resolveColForce * colCondition.y;
				b1Vel.y *= -dinBody->restitution;
			}
		}
	}

	if (colCondition.y > 0 && dinBody->acceleration.y < 0 || colCondition.y < 0 && dinBody->acceleration.y > 0)
	{
		b1.acceleration.y = 0;
	}
	if (colCondition.x > 0 && dinBody->acceleration.x < 0 || colCondition.x < 0 && dinBody->acceleration.x > 0)
	{
		b1.acceleration.x = 0;
	}

	// Si el movimiento es muy flojo, ignora
	b1Vel.x = abs(b1Vel.x) <  1 ? 0 : b1Vel.x;
	b1Vel.y = abs(b1Vel.y) <  1 ? 0 : b1Vel.y;

	dinBody->SetLinearVelocity(b1Vel);
}

fPoint PhysCore::CollisionPoint(RigidBody& b1, RigidBody& b2)
{
	fPoint collisionPoint;

	// Check RECT RECT collision point
	if (b1.shape == ShapeType::RECT && b2.shape == ShapeType::RECT)
	{
		// PUNTO DE COLISION???
		collisionPoint = { b1.velocity.x, b1.velocity.y };

		// col eje X
		if (b1.velocity.y == 0)
		{
			if (b1.velocity.x > 0)
			{
				collisionPoint.x += b1.width;
			}
			else if (b1.velocity.x < 0)
			{
				collisionPoint.x -= b1.width;
			}
		}
		// col eje y
		else if (b1.velocity.x == 0)
		{
			if (b1.velocity.y > 0)
			{
				collisionPoint.x += b1.height;
			}
			else if (b1.velocity.y < 0)
			{
				collisionPoint.x -= b1.height;
			}
		}

		// NO FUNCIONA FALTA CASO DE MOVIMIENTO DIAGONAL
	}
	// Check CIRCLE CIRCLE collision point
	if (b1.shape == ShapeType::CIRCLE && b2.shape == ShapeType::CIRCLE)
	{
		fPoint dir = b1.position - b2.position;

		dir *= -1;
		// PUNTO DE COLISION!!!!!
		collisionPoint = b1.position + dir.Normalize() * b1.radius;
	}
	// Check CIRCLE RECT collision point
	else // CIRCLE col RECT || RECT col CIRCLE
	{
		float height = b2.height * 0.5f;
		float width = b2.width * 0.5f;

		collisionPoint = b1.position - b2.position;

		collisionPoint.x = MAX(-width, MIN(width, collisionPoint.x));
		collisionPoint.y = MAX(-height, MIN(height, collisionPoint.y));

		// PUNTO DE COLISION!!!!!
		collisionPoint = b2.position + collisionPoint;
	}
	return collisionPoint;
}

fPoint PhysCore::CollisionDir(RigidBody& b1, fPoint colPoint)
{
	fPoint dir = b1.position - colPoint;

	dir = dir.Normalize();

	return dir;
}

float PhysCore::submergedVolume(RigidBody* body, RigidBody* water)
{
	if (body->shape == ShapeType::CIRCLE)
	{
		//Obtain the water Y position substracting his height because the pos of the body is in the center
		float waterYpos = water->position.y - water->height / 2;

		float bodySubmergedHeight = body->position.y + body->radius - waterYpos;

		float totalsubmergedarea = bodySubmergedHeight * body->radius * 2;

		totalsubmergedarea /= body->radius * 2 * body->radius * 2;

		return totalsubmergedarea * SQUARETOCIRCLE;
	}

	if (body->shape == ShapeType::RECT)
	{
		//Obtain the water Y position substracting his height because the pos of the body is in the center
		float waterYpos = water->position.y - water->height / 2;

		float bodySubmergedHeight = body->position.y + body->height / 2 - waterYpos;

		float totalsubmergedarea = bodySubmergedHeight * body->width;

		totalsubmergedarea /= body->width * body->height;

		return totalsubmergedarea;
	}
}
