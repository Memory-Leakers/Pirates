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

	return true;
}

bool SceneIntro::PostUpdate()
{
	rect.x = testGO->rBody->GetPosition().x;
	rect.y = testGO->rBody->GetPosition().y;

	_app->renderer->DrawQuad(rect, 255, 0,0, 255);

	rect2.x = testGO2->rBody->GetPosition().x;
	rect2.y = testGO2->rBody->GetPosition().y;

	_app->renderer->DrawQuad(rect2, 255, 255, 0, 255);


	return true;
}
