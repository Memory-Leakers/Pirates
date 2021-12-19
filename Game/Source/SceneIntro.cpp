#include "Globals.h"
#include "Application.h"
#include "SceneIntro.h"
#include "PhysCore.h"
#include "TurnsManager.h"
#include "Water.h"
#include "GameUI.h"

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

	bg[0] = _app->textures->Load("Assets/textures/Background/0.png");
	bg[1] = _app->textures->Load("Assets/textures/Background/1.png");
	bg[2] = _app->textures->Load("Assets/textures/Background/2.png");
	bg[3] = _app->textures->Load("Assets/textures/Background/3.png");

	world = new PhysCore({ 0,10 });
	rectbgclouds = { 0,50 };

	InitScene();

	//BombINit
	/*bombP1 = new Bomb("Bomb", "Bomb", _app, BombType::BANANA);
	bombP1->SetPosition({ 200,180 });

	bombP2 = new Bomb("Bomb", "Bomb", _app, BombType::NORMAL);
	bombP2->SetPosition({ 450,100 });*/

	//player Init

	for (int i = 0; i < 3; i++)
	{
		player1Characters[i] = new Player("Player1", "player", _app, 1);
		player1Characters[i]->rBody = new RigidBody(player1Positions[i], RigidBodyType::DYNAMIC, 11, player1Characters[i]);
		player1Characters[i]->rBody->SetGravityScale(2.0f);
		player1Characters[i]->rBody->SetDragCoeficient(0.1f);
		player1Characters[i]->rBody->SetRestitution(0.2f);
		player1Characters[i]->rBody->SetHydrodynamicDragCoeficient(0.5f);
		player1Characters[i]->rBody->SetFriction(5.0f);

		player2Characters[i] = new Player("Player2", "player", _app, 2);
		player2Characters[i]->rBody = new RigidBody(player2Positions[i], RigidBodyType::DYNAMIC, 11, player2Characters[i]);
		player2Characters[i]->rBody->SetGravityScale(2.0f);
		player2Characters[i]->rBody->SetDragCoeficient(0.1f);
		player2Characters[i]->rBody->SetRestitution(0.2f);
		player2Characters[i]->rBody->SetHydrodynamicDragCoeficient(0.5f);
		player2Characters[i]->rBody->SetFriction(5.0f);
	}

	// Init water
	water = new Water({ 0,450 }, "water", "Water", _app);

	walls[0] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 3200, 5);
	walls[1] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 5, 1440);
	walls[2] = new RigidBody({1550,0 }, RigidBodyType::STATIC, 5, 1440);


	gameUI = new GameUI(_app);
	turnsManager = new TurnsManager(_app, this, world, gameUI);

	for (int i = 0; i < 3; i++)
	{
		gameObjects.add(player1Characters[i]);
		gameObjects.add(player2Characters[i]);
	}

	gameObjects.add(water);

	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->Start();
	}

	for (int i = 0; i < 3; i++)
	{
		world->AddRigidBody(walls[i]);
		world->AddRigidBody(player1Characters[i]->rBody);
		world->AddRigidBody(player2Characters[i]->rBody);
	}

	world->AddRigidBody(water->rBody);

	for (int i = 0; i < 3; i++)
	{
		turnsManager->AddGameObjectAsItem(player1Characters[i], PLAYER1);
		turnsManager->AddGameObjectAsItem(player2Characters[i], PLAYER2);
	}


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
	if (gameUI != nullptr)
	{
		delete gameUI;
		gameUI = nullptr;
	}
	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(bg[i]);
		bg[i] = nullptr;
	}

	Scene::CleanUp();

	return true;
}

// Update: draw background
bool SceneIntro::Update()
{
	world->Update((1.0 / _app->fps));

	turnsManager->UpdateGameLogic();
	gameUI->Update();

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
	//if (_app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	//{
	//	_app->scene->DEBUGMODE = !_app->scene->DEBUGMODE;
	//	if (_app->scene->DEBUGMODE) printf_s("DEBUG ON"); else printf_s("DEBUG OFF");
	//}

	/*if (_app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition({ 200,180 });
			bombP1->setType(BombType::NORMAL);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition({ 450,100 });
			bombP2->setType(BombType::NORMAL);
		}
	}
	if (_app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition({ 200,180 });
			bombP1->setType(BombType::BANANA);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition({ 450,100 });
			bombP2->setType(BombType::BANANA);
		}
	}
	if (_app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		if (turnsManager->currentPlayer == PLAYER1)
		{
			bombP1->active = true;
			bombP1->SetPosition({ 200,180 });
			bombP1->setType(BombType::UMBRELLA);
		}
		else if (turnsManager->currentPlayer == PLAYER2)
		{
			bombP2->active = true;
			bombP2->SetPosition({ 450,100 });
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
	//if (_app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	//{
	//	_app->scene->ChangeCurrentScene(2, 0);
	//}
	if (rectbgclouds.x >= 1200)
	{
		rectbgclouds.x -= 2052;
	}
	else
	{
		rectbgclouds.x++;
	}

	
	_app->renderer->camera->MoveCameraWithMouse();

	return true;
}

bool SceneIntro::PostUpdate()
{
	gameUI->PostUpdate();
	_app->renderer->AddTextureRenderQueue(bg[0], { 0,0 }, { 0,0,0,0 },2.0f);
	_app->renderer->AddTextureRenderQueue(bg[2], { 0,180 }, { 0,0,0,0 }, 2.0f, 0, 0.0f, 0, SDL_FLIP_NONE, 0.4f);
	_app->renderer->AddTextureRenderQueue(bg[1], { 100,100 }, { 0,0,0,0 }, 2.0f, 0, 0.0f, 0, SDL_FLIP_NONE, 0.5f);
	_app->renderer->AddTextureRenderQueue(bg[3], { 0,20 }, { 0,0,0,0 }, 1.0f, 0, 0.0f, 0, SDL_FLIP_NONE, 0.6f);

	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->PostUpdate();
	}

	_app->renderer->AddTextureRenderQueue(bg[3], { rectbgclouds.x,rectbgclouds.y }, { 0,0,0,0 }, 1.0f, 0, 0.0f, 0, SDL_FLIP_NONE, 0.6f);

	return true;
}
