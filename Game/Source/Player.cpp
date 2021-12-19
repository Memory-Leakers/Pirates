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

	tex_dead = _app->textures->Load("Assets/textures/Die/Die.png");

	if (ID == 1)
	{
		tex_health = _app->textures->Load("Assets/textures/UI/HearthBar/unit_health_blue.png");
	}
	else
	{
		tex_health = _app->textures->Load("Assets/textures/UI/HearthBar/unit_health_red.png");
	}
}

void Player::PreUpdate()
{
}

void Player::Update()
{
	if (health <= 0)
	{
		dead = true;
	}

	if (dead)
	{
		if (!anim_dead.HasFinished())
		{
			anim_dead.Update();
		}
	}
	else
	{
		currentAnimation_player->Update();
	}
	
}

void Player::PostUpdate()
{

	position.x = GetWorldPosition().x - (int)((rBody->GetRadius()) * PIXELS_PER_METERS);
	position.y = GetWorldPosition().y - (int)((rBody->GetRadius()) * PIXELS_PER_METERS);
	player = currentAnimation_player->GetCurrentFrame();
	if (ID == 1 && !dead)
	{
		_app->renderer->AddTextureRenderQueue(playerTextures[0],position, player, 1.0f, 2, 0.0f);
		_app->renderer->DrawCircle(rBody->GetPosition().x, rBody->GetPosition().y, rBody->GetRadius()* PIXELS_PER_METERS, 255, 255, 0);
	}
	else if (ID == 2 && !dead)
	{
		_app->renderer->AddTextureRenderQueue(playerTextures[2], position, player, 1.0f, 2, 0.0f);
		_app->renderer->DrawCircle(rBody->GetPosition().x, rBody->GetPosition().y, rBody->GetRadius() * PIXELS_PER_METERS, 255, 255, 0);
	}
	else
	{
		player = anim_dead.GetCurrentFrame();
		_app->renderer->AddTextureRenderQueue(tex_dead, position, player, 1.0f, 2, 0.0f);
	}
	iPoint hBarPoint = position;
	hBarPoint.y -= 8;
	hBarPoint.x -= 2;
	if (!dead)
	{
		switch (health)
		{
		case 0:
		case 1:
			_app->renderer->AddTextureRenderQueue(tex_health, hBarPoint, anim_health.getFrame(2), 1.0f, 2, 0.0f);
			break;
		case 2:
			_app->renderer->AddTextureRenderQueue(tex_health, hBarPoint, anim_health.getFrame(1), 1.0f, 2, 0.0f);
			break;
		case 3:
			_app->renderer->AddTextureRenderQueue(tex_health, hBarPoint, anim_health.getFrame(0), 1.0f, 2, 0.0f);
			break;
		}
	}
}

void Player::CleanUp()
{
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


	//Dead
	anim_dead.PushBack({ 0, 0, 32, 32 });
	anim_dead.PushBack({ 32, 0, 32, 32 });
	anim_dead.PushBack({ 64, 0, 32, 32 });
	anim_dead.PushBack({ 96, 0, 32, 32 });
	anim_dead.PushBack({ 128, 0, 32, 32 });
	anim_dead.PushBack({ 160, 0, 32, 32 });
	anim_dead.PushBack({ 192, 0, 32, 32 });
	anim_dead.PushBack({ 224, 0, 32, 32 });
	anim_dead.PushBack({ 256, 0, 32, 32 });
	anim_dead.PushBack({ 288, 0, 32, 32 });
	anim_dead.hasIdle = false;
	anim_dead.loop = false;
	anim_dead.speed = 0.2;

	//Player health bar
	anim_health.PushBack({ 0, 0, 25, 8 });
	anim_health.PushBack({ 25, 0, 25, 8 });
	anim_health.PushBack({ 50, 0, 25, 8 });
	player_hit.hasIdle = false;
	player_hit.speed = 0.25;
}

void Player::OnCollisionEnter(RigidBody* col)
{
	if (col->GetType() == RigidBodyType::WATER)
	{
		health = 0;
		currentAnimation_player = &anim_dead;
		active = false;
	}
}

void Player::OnCollisionExit(RigidBody* col)
{

}

void Player::OnTriggerEnter(RigidBody* col)
{
}

void Player::OnTriggerExit(RigidBody* col)
{
	
}

void Player::OnTriggerStay(RigidBody* col)
{
	if (dead) return;
	if (col->gObj != nullptr)
	{
		if (col->gObj->tag == "EXPLODED" && !hurt)
		{
			health--;
			hurt = true;
			printf_s("OUCH");

			int colX = col->GetPosition().x;
			int colY = col->GetPosition().y;
			//float mag = 
			fPoint dir = { 0, 0 };

			float forceX = 800.0f;
			float forceY = 800.0f;

			if (colX <= position.x && colY <= position.y)
			{
				dir = { forceX, forceY };
			}
			else if (colX >= position.x && colY <= position.y)
			{
				dir = { -forceX, forceY };
			}
			else if (colX <= position.x && colY >= position.y)
			{
				dir = { forceX, -forceY };
			}
			else if (colX >= position.x && colY >= position.y)
			{
				dir = { -forceX, -forceY };
			}
			rBody->AddForceToCenter(dir);
		}

		if (col->gObj->tag == "Bomb" && hurt)
		{
			hurt = false;
		}
	}
}