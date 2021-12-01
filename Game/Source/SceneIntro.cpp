#include "Globals.h"
#include "Application.h"
#include "SceneIntro.h"
#include "PhysCore.h"

iPoint position;

SceneIntro::SceneIntro(Application* app) : Scene(app)
{}

SceneIntro::~SceneIntro()
{}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	fPoint test = { 2.0,2.0 };

	fPoint temp = test * test;

	printf("%f %f", temp.x, temp.y);

	// Test
	plant.x = 500;
	plant.y = 410;
	plant.w = 50;
	plant.h = 70;

	ground.x = -1000;
	ground.y = 510;
	ground.h = 250;
	ground.w = 3400;

	position.x = 50;
	position.y = 459;

	player.x = position.x;
	player.y = position.y;
	player.w = 50;
	player.h = 50;

	world = new PhysCore({ 0,10 });

	body = new RigidBody({ 300, 300 }, RigidBodyType::DYNAMIC,10,10);

	body2 = new RigidBody({ 400, 300 }, RigidBodyType::DYNAMIC, 10, 10);

	body2->SetGravityScale(2.0f);

	world->AddRigidBody(body);
	
	world->AddRigidBody(body2);
	return ret;
}

// Load assets
bool SceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	RELEASE(world);

	return true;
}

// Update: draw background
bool SceneIntro::Update()
{
	world->Update(1.0/60);

	//printf_s("Position: %f\t %f\n", body->GetPosition().x, body->GetPosition().y);

	if (_app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
	{
		player.x-= 4;
	}

	if (_app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		body->AddForceToCenter({ 0, -20 });
	}

	if (_app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		player.x+= 4;
	}

	int playerCenterX = (player.w / 2) + player.x;
	int plantCenter = (plant.w / 2) + plant.x;
	int distance = abs(playerCenterX - plantCenter);
	if(distance < 35)
	{
		plant.y = 375 + distance;
	
	}

	//1250 , -50
	if (player.x >= 1250) {
		player.x = -49;
	}
	else if (player.x <= -50) 
	{
		player.x = 1249;
	}

	playerCenter = { -playerCenterX,0 };

	return true;
}

bool SceneIntro::PostUpdate()
{

	/*_app->renderer->CameraMove(playerCenter);
	_app->renderer->DrawQuad(ground, 125, 0, 255, 255);
	_app->renderer->DrawQuad(player, 125, 0, 125, 125);
	_app->renderer->DrawQuad(plant, 255, 0, 255, 255);
	_app->renderer->DrawLine(1250, 510, 1250, 300, 255, 0, 0);
	_app->renderer->DrawLine(-50, 510, -50, 300, 0, 255, 0);*/
	
	rect.x = body->GetPosition().x;
	rect.y = body->GetPosition().y;

	_app->renderer->DrawQuad(rect, 255, 0,0, 255);

	rect2.x = body2->GetPosition().x;
	rect2.y = body2->GetPosition().y;

	_app->renderer->DrawQuad(rect2, 255, 255, 0, 255);


	return true;
}
