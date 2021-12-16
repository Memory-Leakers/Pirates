#include "Player.h"




Player::Player(std::string name, std::string tag, Application* _app):GameObject(name,tag,_app)
{
	player_bounds.x = position.x;
	player_bounds.y = position.y;
	player_bounds.w = 32;
	player_bounds.h = 27;

	player1_idle_tex =_app->textures->Load("Assets/text/Pirates1/Idle.png");

	player1_hit_tex = _app->textures->Load("Assets/text/Pirates1/Injure.png");
	
	player2_idle_tex = _app->textures->Load("Assets/text/Pirates2/Idle.png");

	player2_hit_tex = _app->textures->Load("Assets/text/Pirates2/Injure.png");

	SetAnimations();
	currentAnimation_player1 = &player_Idle;

}

Player::~Player()
{
}

void Player::Start()
{
}

void Player::PreUpdate()
{
}

void Player::Update()
{
	currentAnimation_player1->Update();
}

void Player::PostUpdate()
{
	player = player_Idle.GetCurrentFrame();


	_app->renderer->AddTextureRenderQueue(player1_idle_tex, position, player, 1.0f, 2,0.0f);

}

void Player::CleanUp()
{
}

void Player::SetAnimations()
{
	//Idle
	for (int i = 0; i < 4; i++)
	{
		player_Idle.PushBack({ 4+(32*i),4,23,23 });
	}
	
	player_Idle.hasIdle = false;
	player_Idle.speed = 0.25;
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
