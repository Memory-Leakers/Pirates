#include "Player.h"




Player::Player(std::string name, std::string tag, Application* _app,int ID):GameObject(name,tag,_app)
{
	/*player_bounds.x = position.x;
	player_bounds.y = position.y;
	player_bounds.w = 23;
	player_bounds.h = 23;*/
	this->ID = ID;

	SetAnimations();
	currentAnimation_player = &player_Idle;

}

Player::~Player()
{
}

void Player::Start()
{
	//Textures
	playerTextures[0] = _app->textures->Load("Assets/textures/Pirates1/Idle.png");

	playerTextures[1] = _app->textures->Load("Assets/textures/Pirates1/Injure.png");

	playerTextures[2] = _app->textures->Load("Assets/textures/Pirates2/Idle.png");

	playerTextures[3] = _app->textures->Load("Assets/textures/Pirates2/Injure.png");




}

void Player::PreUpdate()
{
}

void Player::Update()
{

	currentAnimation_player->Update();
}

void Player::PostUpdate()
{
	position.x = GetWorldPosition().x - (int)((rBody->GetRadius()) * PIXELS_PER_METERS);
	position.y = GetWorldPosition().y - (int)((rBody->GetRadius()) * PIXELS_PER_METERS);
	player = player_Idle.GetCurrentFrame();
	if (ID == 1)
	{
		_app->renderer->AddTextureRenderQueue(playerTextures[0],position, player, 1.0f, 2, 0.0f);
		_app->renderer->DrawCircle(rBody->GetPosition().x, rBody->GetPosition().y, rBody->GetRadius()* PIXELS_PER_METERS, 255, 255, 0);
	}
	else if (ID == 2)
	{
		_app->renderer->AddTextureRenderQueue(playerTextures[2], position, player, 1.0f, 2, 0.0f);
		_app->renderer->DrawCircle(rBody->GetPosition().x, rBody->GetPosition().y, rBody->GetRadius() * PIXELS_PER_METERS, 255, 255, 0);
	}
}

void Player::CleanUp()
{
	for (int i = 0; i < 4; i++)
	{
		if (playerTextures[i] != nullptr)
		{
			_app->textures->Unload(playerTextures[i]);
			playerTextures[i] = nullptr;
		}
	}
	currentAnimation_player = nullptr;

}

void Player::SetAnimations()
{
	//Idle
	for (int i = 0; i < 4; i++)
	{
		player_Idle.PushBack({ 4+(32*i),4,23,23 });
	}

	player_Idle.hasIdle = false;
	player_Idle.speed = 0.08;
	//Hit

	player_hit.PushBack({0,0,32,27 });
	player_hit.hasIdle = false;
	player_hit.speed = 0.25;

}

void Player::OnCollisionEnter(RigidBody* col)
{
	
}

void Player::OnCollisionExit(RigidBody* col)
{
	
}
