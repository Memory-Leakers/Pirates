#include "Globals.h"
#include "Application.h"
#include "SceneIntro.h"
#include "PhysCore.h"
#include "TurnsManager.h"

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

	testGO = new GameObject("test", "test", _app);
	testGO->rBody = new RigidBody({ 500, 300 }, RigidBodyType::DYNAMIC, 10, 10);
	testGO->rBody->SetGravityScale(0);

	testGO2 = new GameObject("test", "test", _app);
	testGO2->rBody = new RigidBody({ 400, 300 }, RigidBodyType::DYNAMIC, 10, 10);
	testGO2->rBody->SetGravityScale(0);

	turnsManager = new TurnsManager(_app);

	body2->SetGravityScale(2.0f);

	world->AddRigidBody(body);
	
	world->AddRigidBody(body2);

	world->AddRigidBody(testGO->rBody);
	world->AddRigidBody(testGO2->rBody);

	turnsManager->AddGameObjectAsItem(testGO, PLAYER1);
	turnsManager->AddGameObjectAsItem(testGO2, PLAYER2);

	gameObjects.add(testGO);
	gameObjects.add(testGO2);

	_app->ui->CreateUI(1234567890, 500, 500, 3.0f);

	return ret;
}

// Load assets
bool SceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	RELEASE(world);

	if (turnsManager != nullptr)
	{
		turnsManager->CleanUp();
		delete turnsManager;
		turnsManager = nullptr;
	}

	Scene::CleanUp();

	return true;
}

// Update: draw background
bool SceneIntro::Update()
{
	world->Update(1.0/60);

	turnsManager->UpdateGameLogic();

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
	
	rect.x = testGO->rBody->GetPosition().x;
	rect.y = testGO->rBody->GetPosition().y;

	_app->renderer->DrawQuad(rect, 255, 0,0, 255);

	rect2.x = testGO2->rBody->GetPosition().x;
	rect2.y = testGO2->rBody->GetPosition().y;

	_app->renderer->DrawQuad(rect2, 255, 255, 0, 255);


	return true;
}
