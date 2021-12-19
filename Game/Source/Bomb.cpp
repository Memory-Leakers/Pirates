#include "Bomb.h"

Bomb::Bomb(std::string name, std::string tag, Application* _app, BombType bType) : GameObject(name, tag, _app)
{
	this->name = name;
	this->tag = tag;
	this->_app = _app;
	this->bType = bType;

	tex_normal = _app->textures->Load("Assets/textures/Bomb/CherryBomb/Idle.png");
	tex_banana = _app->textures->Load("Assets/textures/Bomb/Banana/Banana.png");
	tex_umbrella = _app->textures->Load("Assets/textures/Bomb/Umbrella/Umbrella.png");
	tex_explosion = _app->textures->Load("Assets/textures/Bomb/Explosion.png");

	SetAnimations();
}

Bomb::Bomb(GameObject& bomb) : GameObject(bomb)
{
	this->name = bomb.name;
	this->tag = bomb.tag;
	this->bType = bType;
}

Bomb::~Bomb()
{

}

void Bomb::OnCollisionEnter(RigidBody* col)
{
	if (rBody->GetGravityScale() <= 0.0f) return;
	if (!active) return;
	if (col->gObj == nullptr)
	{
		if (!exploded)
		{
			switch (bType)
			{
			case BombType::UMBRELLA:
			case BombType::NORMAL:			
				exploded = true;
				break;
			case BombType::BANANA:
				bounceCount--;
				if (bounceCount <= 0)
				{
					exploded = true;
				}
				break;
			}
		}
	}
	
}

void Bomb::OnCollisionExit(RigidBody* col)
{
	
}

void Bomb::OnTriggerEnter(RigidBody* col)
{

}

void Bomb::Start()
{
	//Bomb body
	rBody = new RigidBody(position, RigidBodyType::DYNAMIC, bombRadius, this);
	rBody->SetGravityScale(0.0f);

	//Explosion area (body)
	rBodyTrigger = new RigidBody(position, RigidBodyType::DYNAMIC, explosionRadius, this, COL_TYPE::TRIGGER);
	rBodyTrigger->SetGravityScale(0.0f);
}

void Bomb::PreUpdate()
{
	if (!active) return;
}

void Bomb::Update()
{
	if (!exploded)
	{
		rBodyTrigger->SetPosition(rBody->GetPosition());
	}

	anim_normal.Update();
	anim_umbrella.Update();
	
	position.x = GetWorldPosition().x;
	position.y = GetWorldPosition().y;
}

void Bomb::PostUpdate()
{
	if (!active) return;
	bounds.x = position.x;
	bounds.y = position.y;

	if (exploded)
	{
		anim_explosion.Update();
		position.x -= 64;
		position.y -= 64;
		rect = anim_explosion.GetCurrentFrame();
		_app->renderer->AddTextureRenderQueue(tex_explosion, position, rect, 4.0f, 2, 0.0f);
		tag = "EXPLODED";
		rBody->SetGravityScale(0.0f);
		rBody->SetRestitution(0.0f);
		rBody->SetLinearVelocity({ 0.0f, 0.0f });
		if (anim_explosion.getCurrentFrameI() == anim_explosion.size() - 1)
		{
			exploded = false;
			active = false;
			rBodyTrigger->SetPosition({ 0 , 0});
			tag = "Bomb";
			pendingToDelete = true;
			anim_explosion.Reset();
		}
	}
	else
	{
		switch(bType)
		{
			case BombType::NORMAL:
				position.x -= 16;
				position.y -= 24;
				rect = anim_normal.GetCurrentFrame();
				//rBody->SetGravityScale(2.0f);
				rBody->SetRestitution(0.6f);
				rBody->SetDragCoeficient(0.09f);
				_app->renderer->AddTextureRenderQueue(tex_normal, position, rect, 1.0f, 2, 0.0f);
				break;
			case BombType::BANANA:
				position.x -= 13;
				position.y -= 7;
				rect = anim_banana.GetCurrentFrame();
				//rBody->SetGravityScale(2.0f);
				rBody->SetRestitution(0.6f);
				rBody->SetFriction(-8.0f);
				rBody->SetDragCoeficient(0.08f);
				_app->renderer->AddTextureRenderQueue(tex_banana, position, rect, 1.0f, 2, 0.0f);
				break;
			case BombType::UMBRELLA:
				position.x -= 17;
				position.y -= 46;
				rect = anim_umbrella.GetCurrentFrame();
				//rBody->SetGravityScale(2.0f);
				rBody->SetRestitution(0);
				rBody->SetDragCoeficient(0.22f);
				_app->renderer->AddTextureRenderQueue(tex_umbrella, position, rect, 1.0f, 2, 0.0f);
				break;
		}
		//tag = "Bomb";
	}
	//Explosion radius
	if (_app->scene->DEBUGMODE)
	{
		_app->renderer->DrawCircle(bounds.x, bounds.y, bombRadius, 255, 0, 0);
		bounds.x = rBodyTrigger->GetPosition().x;
		bounds.y = rBodyTrigger->GetPosition().y;
		_app->renderer->DrawCircle(bounds.x, bounds.y, explosionRadius, 255, 0, 0);
	}
}

void Bomb::CleanUp()
{
	rBody = nullptr;
}

void Bomb::setType(BombType type)
{
	this->bType = type;
}

void Bomb::SetAnimations()
{
	anim_normal.PushBack({ 0, 0, 32, 32 });
	anim_normal.PushBack({ 32, 0, 32, 32 });
	anim_normal.PushBack({ 64, 0, 32, 32 });
	anim_normal.PushBack({ 96, 0, 32, 32 });
	anim_normal.hasIdle = false;
	anim_normal.speed = 0.25;

	anim_banana.PushBack({ 0, 0, 28, 16 });
	anim_banana.hasIdle = true;
	anim_banana.speed = 0.25;

	anim_umbrella.PushBack({ 0, 0, 34, 54 });
	anim_umbrella.PushBack({ 34, 0, 34, 54 });
	anim_umbrella.PushBack({ 68, 0, 34, 54 });
	anim_umbrella.PushBack({ 102, 0, 34, 54 });
	anim_umbrella.hasIdle = false;
	anim_umbrella.speed = 0.25;

	anim_explosion.PushBack({ 0, 0, 32, 32 });
	anim_explosion.PushBack({ 32, 0, 32, 32 });
	anim_explosion.PushBack({ 64, 0, 32, 32 });
	anim_explosion.PushBack({ 96, 0, 32, 32 });
	anim_explosion.PushBack({ 128, 0, 32, 32 });
	anim_explosion.PushBack({ 160, 0, 32, 32 });
	anim_explosion.PushBack({ 192, 0, 32, 32 });
	anim_explosion.hasIdle = false;
	anim_explosion.speed = 0.2;
	//anim_explosion.speed = 0.05;
}
