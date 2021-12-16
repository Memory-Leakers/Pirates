#include "Bomb.h"

Bomb::Bomb(std::string name, std::string tag, Application* _app, BombType bType) : GameObject(name, tag, _app)
{
	this->name = name;
	this->tag = tag;
	this->_app = _app;
	this->bType = bType;

	tex_normal = _app->textures->Load("Assets/textures/Bomb/CherryBomb/Idle.png");
	tex_banana = _app->textures->Load("Assets/textures/Bomb/Banana/Banana.png");
	tex_umbrella = _app->textures->Load("Assets/textures/Bomb/CherryBomb/CherryBomb.png");

	SetAnimations();
	anim_current = &anim_normal;
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
	printf_s("BOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMB");
}

void Bomb::OnCollisionExit(RigidBody* col)
{
	printf_s("BOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMB");
}

void Bomb::OnTriggerEnter(RigidBody* col)
{
	printf_s("BOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMBBOMB");
}

void Bomb::Start()
{
	//Cargar texturas
	rBody = new RigidBody(position, RigidBodyType::DYNAMIC, radius);
	rBody->SetGravityScale(2.0f);
	rBody->SetRestitution(0.6f);
	rBody->SetDragCoeficient(0.15f);
}

void Bomb::PreUpdate()
{
	if (!active) return;
}

void Bomb::Update()
{
	if (!active) return;

	anim_current->Update();
	position.x = GetWorldPosition().x;
	position.y = GetWorldPosition().y;
}

void Bomb::PostUpdate()
{
	if (!active) return;
	bounds.x = position.x;
	bounds.y = position.y;


	switch(bType)
	{
		case BombType::NORMAL:
			position.x -= 16;
			position.y -= 24;
			rect = anim_normal.GetCurrentFrame();
			_app->renderer->AddTextureRenderQueue(tex_normal, position, rect, 1.0f, 2, 0.0f);
			break;
		case BombType::BANANA :
			position.x -= 13;
			position.y -= 7;
			rect = anim_banana.GetCurrentFrame();
			_app->renderer->AddTextureRenderQueue(tex_banana, position, rect, 1.0f, 2, 0.0f);
			break;
		case BombType::MINI_BANANA:
			_app->renderer->AddTextureRenderQueue(tex_banana, position, rect, 1.0f, 2, 0.0f);
			break;
		case BombType::UMBRELLA:
			rect = anim_umbrella.GetCurrentFrame();
			_app->renderer->AddTextureRenderQueue(tex_umbrella, position, rect, 1.0f, 2, 0.0f);
			break;
	}

	//Explosion radius
	if (_app->scene->DEBUGMODE)
	{
		_app->renderer->DrawCircle(bounds.x, bounds.y, explosionRadius, 255, 0, 0);
		_app->renderer->DrawCircle(bounds.x, bounds.y, radius, 255, 0, 0);
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
}