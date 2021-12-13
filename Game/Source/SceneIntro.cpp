#include "Globals.h"
#include "Application.h"
#include "SceneIntro.h"
#include "PhysCore.h"
#include "TurnsManager.h"

SceneIntro::SceneIntro(Application* app) : Scene(app)
{}

SceneIntro::~SceneIntro()
{}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	world = new PhysCore({ 0,10 });

	testGO = new GameObject("test", "test", _app);
	testGO->rBody = new RigidBody({ 300, 180 }, RigidBodyType::DYNAMIC, 10);
	testGO->rBody->SetGravityScale(1.0f);
	testGO->rBody->SetRestitution(0.6f);
	testGO->rBody->SetDragCoeficient(0.01f);

	testGO2 = new GameObject("test", "test", _app);
	testGO2->rBody = new RigidBody({ 400, 180 }, RigidBodyType::DYNAMIC, 10);
	testGO2->rBody->SetGravityScale(1.0f);

	floor = new GameObject("test", "test", _app);
	floor->rBody = new RigidBody({ 0, 600 }, RigidBodyType::STATIC, 1280, 300);

	turnsManager = new TurnsManager(_app);

	world->AddRigidBody(floor->rBody);

	world->AddRigidBody(testGO->rBody);
	world->AddRigidBody(testGO2->rBody);

	turnsManager->AddGameObjectAsItem(testGO, PLAYER1);
	turnsManager->AddGameObjectAsItem(testGO2, PLAYER2);

	gameObjects.add(testGO);
	gameObjects.add(testGO2);
	gameObjects.add(floor);

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

	// Camera Follow Logic
	if (turnsManager->throwedGameObj != nullptr)		// Follow throwed Game Object
	{
		_app->renderer->camera->SetTarget(turnsManager->throwedGameObj);
	}
	else 
	{
		_app->renderer->camera->SetTarget(nullptr);
	}

	if (_app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		testGO->rBody->SetLinearVelocity({ 0,0 });
	}

	_app->renderer->camera->MoveCameraWithMouse();

	return true;
}

bool SceneIntro::PostUpdate()
{
	
	rect3.x = floor->GetScreenPosition().x;
	rect3.y = floor->GetScreenPosition().y -150;
	rect3.w = 1280;
	rect3.h = 300;

	_app->renderer->AddRectRenderQueue(rect3, 0, 255, 255);

	rect.x = testGO->GetScreenPosition().x;
	rect.y = testGO->GetScreenPosition().y;

	if (_app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		printf("ItemX = %d\n", rect.x);
		printf("ItemY = %d\n", rect.y);
		//printf("MouseX = %d\n", _app->input->GetMouseX());
		//printf("MouseY = %d\n", _app->input->GetMouseY());
	}


	_app->renderer->AddRectRenderQueue(rect, 255, 0, 0);

	rect2.x = testGO2->GetScreenPosition().x;
	rect2.y = testGO2->GetScreenPosition().y;

	_app->renderer->AddRectRenderQueue(rect2, 255, 255, 0);

	return true;
}
