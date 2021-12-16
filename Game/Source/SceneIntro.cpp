#include "Globals.h"
#include "Application.h"
#include "SceneIntro.h"
#include "PhysCore.h"
#include "TurnsManager.h"
#include "Water.h"

SceneIntro::SceneIntro(Application* app) : Scene(app)
{}

SceneIntro::~SceneIntro()
{}

// Load assets
bool SceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	_app->map->Load("PiratesLevel.tmx");

	world = new PhysCore({ 0,10 });

	InitScene();

	//BombINit
	bomb = new Bomb("Bomb", "Bomb", _app, BombType::BANANA);
	bomb->SetPosition({ 200,180 });

	//player Init
	player1 = new Player("Player1", "player", _app,1);
	player1->Start();
	player1->rBody = new RigidBody({ 230,180 }, RigidBodyType::DYNAMIC, 11);
	player1->rBody->SetGravityScale(1.0f);
	player1->rBody->SetRestitution(0.2f);

	player2 = new Player("Player2", "player", _app, 2);
	player2->Start();
	player2->rBody = new RigidBody({ 250,180 }, RigidBodyType::DYNAMIC, 11);
	player2->rBody->SetGravityScale(1.0f);
	player2->rBody->SetRestitution(0.2f);

	// Init water
	water = new Water({ 0,450 }, "water", "Water", _app);

	walls[0] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 3200, 5);
	walls[1] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 5, 1440);
	walls[2] = new RigidBody({1550,0 }, RigidBodyType::STATIC, 5, 1440);

	turnsManager = new TurnsManager(_app);

	gameObjects.add(bomb);
	gameObjects.add(player1);
	gameObjects.add(player2);
	gameObjects.add(water);

	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->Start();
	}

	for (int i = 0; i < 3; i++)
	{
		world->AddRigidBody(walls[i]);
	}

	world->AddRigidBody(bomb->rBody);
	world->AddRigidBody(player1->rBody);
	world->AddRigidBody(player2->rBody);
	world->AddRigidBody(water->rBody);

	turnsManager->AddGameObjectAsItem(player1, PLAYER1);
	turnsManager->AddGameObjectAsItem(player2, PLAYER2);

	return ret;
}

void SceneIntro::InitScene()
{
	// Obstacles
	for (int i = 0; i < _app->map->mapObjects.count(); i++)
	{
		if (_app->map->mapObjects[i].id == 0)
		{
			GameObject* g = new GameObject("wall", "Wall", _app);
			// +8 = offset, porque pivot de b2Body es el centro, y de tectura es izquierda superior.
			g->rBody = new RigidBody({ _app->map->mapObjects[i].position.x +8, _app->map->mapObjects[i].position.y +8 }, RigidBodyType::STATIC, 16, 16);
			world->AddRigidBody(g->rBody);
			gameObjects.add(g);
		}
	}
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

	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->Update();
	}

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

	if (_app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		_app->scene->DEBUGMODE = !_app->scene->DEBUGMODE;
		if (_app->scene->DEBUGMODE) printf_s("DEBUG ON"); else printf_s("DEBUG OFF");
	}

	_app->renderer->camera->MoveCameraWithMouse();

	return true;
}

bool SceneIntro::PostUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PostUpdate();
	}

	_app->renderer->AddRectRenderQueue({ 0,0,3200,5 }, 255, 0, 0);
	_app->renderer->AddRectRenderQueue({ 1550,0,5,1440 }, 255, 0, 0);
	_app->renderer->AddRectRenderQueue({ 0,0,5,1440 }, 255, 0, 0);

	return true;
}
