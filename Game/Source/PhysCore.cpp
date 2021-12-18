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

		// Check if collision Stay
		for (int j = 0; j < rigidBodies[i]->collisionList.count(); j++)
		{
			// Trigger
			if (rigidBodies[i]->colType == COL_TYPE::TRIGGER || rigidBodies[i]->collisionList[j]->colType == COL_TYPE::TRIGGER)
			{
				rigidBodies[i]->OnTriggerStay(rigidBodies[i]->collisionList[j]);
			}
			// Collision
			else
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

					//printf("%f\n", submerge);

					// *2 = 200% para que pueda subir
					magnitudbuoyancy = density * mod * submerge * 2 * rigidBodies[i]->GetGravityScale();

					buoyancyForce = direction * magnitudbuoyancy;

					rigidBodies[i]->AddForceToCenter(buoyancyForce);

					// Calcular la fuerza de drag hidrodinamica
					dragForce = (rigidBodies[i]->velocity * -1) * rigidBodies[i]->hydrodynamicDrag;

					rigidBodies[i]->AddForceToCenter(dragForce);
				}
				else
				{
					// Clipping case!!!
					if (rigidBodies[i]->colType == COL_TYPE::COLLISION && rigidBodies[i]->collisionList[j]->type == RigidBodyType::STATIC)
					{
						if (rigidBodies[i]->collisionList[j]->Contains(rigidBodies[i]->position))
						ResolveClipping(*rigidBodies[i], *rigidBodies[i]->collisionList[j]);
					}
					//FUERZA DE FRICCIÓN
					if (rigidBodies[i]->collisionList[j]->type == RigidBodyType::STATIC)
					{
						dragForce = (rigidBodies[i]->GetLinearVelocity() * -1) * rigidBodies[i]->GetFriction();

						rigidBodies[i]->AddForceToCenter(dragForce);
					}
					//IMPEDIR QUE ENTRE DENTRO DE UN RIGIDBODY
					fPoint colPoint = CollisionPoint(*rigidBodies[i], *rigidBodies[i]->collisionList[j]);
					ResolveColForce(*rigidBodies[i], *rigidBodies[i]->collisionList[j], colPoint);

				}
			}
		}

		// If velocity is few, ignore
		if (abs(rigidBodies[i]->velocity.x) < 0.1f) rigidBodies[i]->velocity.x = 0;
		if (abs(rigidBodies[i]->velocity.y) < 0.1f) rigidBodies[i]->velocity.y = 0;

		// Save lastPosition
		rigidBodies[i]->lastPosition = rigidBodies[i]->position;
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

void PhysCore::BoxColBox(RigidBody& b1, RigidBody& b2)
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
				if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
				{
					b1.OnCollisionExit(&b2);
				}
				else
				{
					b1.OnTriggerExit(&b2);
				}

				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));
				return;
			}
		}
	}

	// Collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		// Collision stay
		if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
		{
			if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
			{
				b1.OnCollisionStay(&b2);
				ResolveColForce(b1, b2, CollisionPoint(b1, b2));
			}
			else
			{
				b1.OnTriggerStay(&b2);
			}
			return;
		}
	}

	// Collision Enter
	if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
	{
		b1.OnCollisionEnter(&b2);
		ResolveColForce(b1, b2, CollisionPoint(b1, b2));
	}
	else
	{
		b1.OnTriggerEnter(&b2);
	}
	b1.collisionList.add(&b2);

	return;
}

void PhysCore::CircleColCircle(RigidBody& b1, RigidBody& b2)
{
	float distX = b1.position.x - b2.position.x;
	float distY = b1.position.y - b2.position.y;
	float distance = sqrt(pow(distX, 2) + pow(distY, 2));

	//Collision
	if (distance <= b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
				{
					b1.OnCollisionStay(&b2);
					fPoint colPoint = CollisionPoint(b1, b2);
					ResolveColForce(b1, b2, colPoint);
				}
				else
				{
					b1.OnTriggerStay(&b2);
				}

				return;
			}
		}

		b1.collisionList.add(&b2);

		if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
		{
			b1.OnCollisionEnter(&b2);
			fPoint colPoint = CollisionPoint(b1, b2);
			ResolveColForce(b1, b2, colPoint);
		}
		else
		{
			b1.OnTriggerEnter(&b2);
		}

		return;
	}

	//No Collision
	if (distance > b1.radius + b2.radius)
	{
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
				{
					b1.OnCollisionExit(&b2);
				}
				else
				{
					b1.OnTriggerExit(&b2);
				}

				b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));

				return;
			}
		}
	}
}

void PhysCore::BoxColCircle(RigidBody& b1, RigidBody& b2)
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
		for (int i = 0; i < b1.collisionList.count(); i++)
		{
			if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
			{
				if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
				{
					b1.OnCollisionStay(&b2);
					ResolveColForce(b1, b2, colPoint);
					return;
				}
				else
				{
					b1.OnTriggerStay(&b2);
					return;
				}
			}
		}

		b1.collisionList.add(&b2);

		if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
		{
			b1.OnCollisionEnter(&b2);
			ResolveColForce(b1, b2, colPoint);
		}
		else
		{
			b1.OnTriggerEnter(&b2);
		}
		return;
	}

	// No collision case
	for (int i = 0; i < b1.collisionList.count(); i++)
	{
		if (rigidBodies.find(b1.collisionList[i]) == rigidBodies.find(&b2))
		{
			if (b1.colType == COL_TYPE::COLLISION && b2.colType == COL_TYPE::COLLISION)
			{
				b1.OnCollisionExit(&b2);
			}
			else
			{
				b1.OnTriggerExit(&b2);
			}

			b1.collisionList.remove(b1.collisionList.At(b1.collisionList.find(&b2)));

			return;
		}
	}
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
		//printf("Canno't resolve collision force!");
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
			//printf("Can not resolve force rect & rect");
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

void PhysCore::ResolveClipping(RigidBody& b1, RigidBody& b2)
{
	RigidBody* dinBody = nullptr;
	RigidBody* staticBody = nullptr;

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

	// Just resolve circle && rect clamping
	if (dinBody->shape != ShapeType::CIRCLE || staticBody->shape != ShapeType::RECT)
	{
		printf("Can not resolve clamping");
		return;
	}

	// calcular interseccion de linea de last position->position y static body
	// circle trayector ray
	fPoint p1 = dinBody->lastPosition;
	fPoint p2 = dinBody->position;

	// rect line 1
	fPoint p3 = { staticBody->position.x - (staticBody->width / 2), staticBody->position.y - (staticBody->height / 2) };
	fPoint p4 = p3;
	p4.x += staticBody->width;

	fPoint colPoint = IntersectionPoint(p1, p2, p3, p4);

	if (colPoint.x != 0 && colPoint.y != 0)
	{
		fPoint dir = p2 - p1;
		dir = dir.Normalize();
		dir.y = -dir.y;
		dinBody->position = colPoint + dir * dinBody->radius;
		ResolveColForce(*dinBody, *staticBody, colPoint);
		return;
	}

	// rect line 2
	p3 = { staticBody->position.x - (staticBody->width / 2), staticBody->position.y - (staticBody->height / 2) };
	p4 = p3;
	p4.y += staticBody->height;

	colPoint = IntersectionPoint(p1, p2, p3, p4);

	if (colPoint.x != 0 && colPoint.y != 0)
	{
		fPoint dir = p2 - p1;
		dir = dir.Normalize();
		dir.y = -dir.y;
		dinBody->position = colPoint + dir * dinBody->radius;
		ResolveColForce(*dinBody, *staticBody, colPoint);
		return;
	}

	// rect line 3
	p3 = { staticBody->position.x - (staticBody->width / 2), staticBody->position.y - (staticBody->height / 2) };
	p3.y += staticBody->height;
	p4 = p3;
	p4.x += staticBody->width;

	colPoint = IntersectionPoint(p1, p2, p3, p4);

	if (colPoint.x != 0 && colPoint.y != 0)
	{
		fPoint dir = p2 - p1;
		dir = dir.Normalize();
		dir.y = -dir.y;
		dinBody->position = colPoint + dir * dinBody->radius;
		ResolveColForce(*dinBody, *staticBody, colPoint);
		return;
	}

	// rect line 4
	p3 = { staticBody->position.x - (staticBody->width / 2), staticBody->position.y - (staticBody->height / 2) };
	p3.x += staticBody->width;
	p4 = p3;
	p4.y += staticBody->height;

	colPoint = IntersectionPoint(p1, p2, p3, p4);

	if (colPoint.x != 0 && colPoint.y != 0)
	{
		fPoint dir = p2 - p1;
		dir = dir.Normalize();
		dir.y = -dir.y;
		dinBody->position = colPoint + dir * dinBody->radius;
		ResolveColForce(*dinBody, *staticBody, colPoint);
		return;
	}
}

fPoint PhysCore::IntersectionPoint(fPoint p1, fPoint p2, fPoint p3, fPoint p4)
{
	fPoint colPoint = { 0, 0 };

	float lambda = (((p4.x - p2.x) * (p3.y - p4.y)) - ((p4.y - p2.y) * (p3.x - p4.x))) / (((p1.x - p2.x) * (p3.y - p4.y)) - ((p1.y - p2.y) * (p3.x - p4.x)));

	if (IN_RANGE(lambda, 0, 1))
	{
		colPoint.x = lambda * p1.x + (1 - lambda) * (p2.x);
		colPoint.y = lambda * p1.y + (1 - lambda) * (p2.y);
	}

	//printf("x = %f \t y= %f\n", colPoint.x, colPoint.y);

	return colPoint;
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
