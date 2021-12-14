#include "Bomb.h"

Bomb::Bomb(std::string name, std::string tag, Application* _app, BombType bType) : GameObject(name, tag, _app)
{
	this->name = name;
	this->tag = tag;
	this->_app = _app;
	this->bType = bType;
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
}

void Bomb::OnCollisionExit(RigidBody* col)
{

}

void Bomb::OnTriggerEnter(RigidBody* col)
{
}

void Bomb::Start()
{
	//Cargar texturas
	rBody = new RigidBody(position, RigidBodyType::DYNAMIC, 10);
	rBody->SetGravityScale(2.0f);
	rBody->SetRestitution(0.6f);
	rBody->SetDragCoeficient(0.01f);
}

void Bomb::PreUpdate()
{
}

void Bomb::Update()
{
}

void Bomb::PostUpdate()
{
	rect.x = GetWorldPosition().x;
	rect.y = GetWorldPosition().y;

	switch(bType)
	{
		case BombType::NORMAL:
			_app->renderer->AddRectRenderQueue(rect, 255, 0, 255);
			break;
		case BombType::BANANA :
			_app->renderer->AddRectRenderQueue(rect, 0, 0, 255);
			break;
		case BombType::MINI_BANANA:
			_app->renderer->AddRectRenderQueue(rect, 255, 0, 0);
			break;
		case BombType::UMBRELLA:
			_app->renderer->AddRectRenderQueue(rect, 0, 255, 0);
			break;
	}

	//Explosion radius
	if (_app->scene->DEBUGMODE)
	{
		_app->renderer->DrawCircle(rect.x, rect.y, explosionRadius, 255, 0, 0);
	}
}

void Bomb::CleanUp()
{
}