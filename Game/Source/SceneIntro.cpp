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
	/*bombP1 = new Bomb("Bomb", "Bomb", _app, BombType::BANANA);
	bombP1->SetPosition({ 200,180 });

	bombP2 = new Bomb("Bomb", "Bomb", _app, BombType::NORMAL);
	bombP2->SetPosition({ 450,100 });*/

	//player Init
	player1 = new Player("Player1", "player", _app,1);
	player1->rBody = new RigidBody({ 230,180 }, RigidBodyType::DYNAMIC, 11, player1);
	player1->rBody->SetGravityScale(2.0f);
	player1->rBody->SetDragCoeficient(0.1f);
	player1->rBody->SetRestitution(0.2f);

	player2 = new Player("Player2", "player", _app, 2);
	player2->rBody = new RigidBody({ 250,180 }, RigidBodyType::DYNAMIC, 11, player2);
	player2->rBody->SetGravityScale(2.0f);
	player2->rBody->SetDragCoeficient(0.1f);
	player2->rBody->SetRestitution(0.2f);

	//BombInit
	bombP1 = new Bomb("Bomb", "Bomb", _app, BombType::BANANA);
	bombP1->SetPosition(player1->rBody->GetPosition());

	bombP2 = new Bomb("Bomb", "Bomb", _app, BombType::NORMAL);
	bombP2->SetPosition(player2->rBody->GetPosition());

	// Init water
	water = new Water({ 0,450 }, "water", "Water", _app);

	walls[0] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 3200, 5);
	walls[1] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 5, 1440);
	walls[2] = new RigidBody({1550,0 }, RigidBodyType::STATIC, 5, 1440);

	turnsManager = new TurnsManager(_app, this, world);

	//gameObjects.add(bombP1);
	//gameObjects.add(bombP2);
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

	//world->AddRigidBody(bombP1->rBody);
	//world->AddRigidBody(bombP2->rBody);
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

bool SceneIntro::PreUpdate()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PreUpdate();
		if (gameObjects[i]->pendingToDelete == true)
		{
			if (gameObjects[i]->rBody != nullptr)
			{
				world->DeleteRigidBody(gameObjects[i]->rBody);
			}
			gameObjects[i]->CleanUp();
			gameObjects.del(gameObjects.At(gameObjects.find(gameObjects[i])));
		}
	}

	return true;
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
	/*else if (turnsManager->changingTurn)
	{
		float distance = (turnsManager->playerItems[turnsManager->currentPlayer][0]->gameObject->GetScreenPosition() - iPoint(_app->renderer->camera->x, _app->renderer->camera->y)).Module();

		if (distance > 5.0f && !_app->renderer->camera->reachedMax)
		{
			_app->renderer->camera->SetTarget(turnsManager->playerItems[turnsManager->currentPlayer][0]->gameObject);
		}
		else
		{
			turnsManager->changingTurn = false;
			_app->renderer->camera->SetTarget(nullptr);
		}
	}*/
	else
	{
		_app->renderer->camera->SetTarget(nullptr);
	}

	// Debug
	if (_app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		_app->scene->DEBUGMODE = !_app->scene->DEBUGMODE;
		if (_app->scene->DEBUGMODE) printf_s("DEBUG ON"); else printf_s("DEBUG OFF");
	}

	/*if (_app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition(player1->GetDrawPosition());
			bombP1->setType(BombType::NORMAL);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition(player2->GetDrawPosition());
			bombP2->setType(BombType::NORMAL);
		}
	}
	if (_app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition(player1->GetDrawPosition());
			bombP1->setType(BombType::BANANA);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition(player2->GetDrawPosition());
			bombP2->setType(BombType::BANANA);
		}
	}
	if (_app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition(player1->GetDrawPosition());
			bombP1->setType(BombType::UMBRELLA);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition(player2->GetDrawPosition());
			bombP2->setType(BombType::UMBRELLA);
		}
	}
	*/
	/*if (turnsManager->currentPlayer == PLAYER1)
	{
		//bombP1->active = true;
		bombP2->active = false;
	}
	else
	{
		//bombP2->active = true;
		bombP1->active = false;
	}*/
	if (_app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		_app->scene->ChangeCurrentScene(2, 0);
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
