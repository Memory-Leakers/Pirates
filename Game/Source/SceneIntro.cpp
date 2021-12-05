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
	
	rect.x = testGO->GetScreenPosition().x;
	rect.y = testGO->GetScreenPosition().y;

	if (_app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		printf("ItemX = %d\n", rect.x);
		printf("ItemY = %d\n", rect.y);
		printf("MouseX = %d\n", _app->input->GetMouseX());
		printf("MouseY = %d\n", _app->input->GetMouseY());
	}


	_app->renderer->AddRectRenderQueue(rect, 255, 0, 0);

	rect2.x = testGO2->GetScreenPosition().x;
	rect2.y = testGO2->GetScreenPosition().y;

	_app->renderer->AddRectRenderQueue(rect2, 255, 255, 0);

	rect3.x = _app->renderer->camera->x;
	rect3.y = _app->renderer->camera->y;

	_app->renderer->AddRectRenderQueue(rect3, 0, 0, 0);

	return true;
}
