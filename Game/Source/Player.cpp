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

	iPoint hBarPoint = position;
	hBarPoint.y -= 8;
	hBarPoint.x -= 2;
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


	//Player health bar
	anim_health.PushBack({ 0, 0, 25, 8 });
	anim_health.PushBack({ 25, 0, 49, 8 });
	anim_health.PushBack({ 50, 0, 75, 8 });
	player_hit.hasIdle = false;
	player_hit.speed = 0.25;
}

void Player::OnCollisionEnter(RigidBody* col)
{
	if (col->gObj != nullptr)
	{
		if (col->gObj->tag == "EXPLODED" && !hurt)
		{
			health--;
			hurt = true;
			printf("OUCH");

			int colX = col->GetPosition().x;
			int colY = col->GetPosition().y;
			//float mag = 
			fPoint dir = { 0, 0 };

			if (colX <= position.x && colY <= position.y)
			{
				dir = { -200, -200 };
			}
			else if (colX >= position.x && colY <= position.y)
			{
				dir = { 200, -200 };
			}
			else if (colX <= position.x && colY >= position.y)
			{
				dir = { -200, 200 };
			}
			else if (colX >= position.x && colY >= position.y)
			{
				dir = { 200, 200 };
			}
			rBody->AddForceToCenter(dir);
		}
	}
}

void Player::OnCollisionExit(RigidBody* col)
{
	if (col->gObj != nullptr)
	{
		if (col->gObj->tag == "EXPLODED" && hurt)
		{
			hurt = false;
			printf("NOUCH");
		}
	}
}
