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

	_app->map->Load("PiratesLevel.tmx");

	world = new PhysCore({ 0,10 });

	InitScene();

	//BombINit
	bomb = new Bomb("Bomb", "Bomb", _app, BombType::BANANA);
	bomb->Start();
	bomb->SetPosition({ 200,180 });

	//player Init
	player1 = new Player("Player1", "player", _app,1);
	player1->Start();
	player1->rBody = new RigidBody({ 230,180 }, RigidBodyType::DYNAMIC, 11);
	player1->rBody->SetGravityScale(2.0f);
	player1->rBody->SetDragCoeficient(0.1f);
	player1->rBody->SetRestitution(0.2f);

	player2 = new Player("Player2", "player", _app, 2);
	player2->Start();
	player2->rBody = new RigidBody({ 250,180 }, RigidBodyType::DYNAMIC, 11);
	player2->rBody->SetGravityScale(2.0f);
	player2->rBody->SetDragCoeficient(0.1f);
	player2->rBody->SetRestitution(0.2f);


	testGO = new GameObject("test", "test", _app);
	testGO->rBody = new RigidBody({ 300, 180 }, RigidBodyType::DYNAMIC, 5);
	testGO->rBody->SetGravityScale(1.0f);
	testGO->rBody->SetRestitution(0.6f);
	testGO->rBody->SetDragCoeficient(0.01f);

	testGO2 = new GameObject("test", "test", _app);
	testGO2->rBody = new RigidBody({ 400, 180 }, RigidBodyType::DYNAMIC, 5);
	testGO2->rBody->SetGravityScale(1.0f);

	walls[0] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 3200, 5);
	walls[1] = new RigidBody({ 0,0 }, RigidBodyType::STATIC, 5, 1440);
	walls[2] = new RigidBody({1550,0 }, RigidBodyType::STATIC, 5, 1440);

	turnsManager = new TurnsManager(_app);

	for (int i = 0; i < 3; i++)
	{
		world->AddRigidBody(walls[i]);
	}

	world->AddRigidBody(testGO->rBody);
	world->AddRigidBody(testGO2->rBody);
	world->AddRigidBody(bomb->rBody);
	world->AddRigidBody(player1->rBody);
	world->AddRigidBody(player2->rBody);

	//turnsManager->AddGameObjectAsItem(bomb, PLAYER1);
	//turnsManager->AddGameObjectAsItem(testGO, PLAYER1);
	//turnsManager->AddGameObjectAsItem(testGO2, PLAYER2);
	turnsManager->AddGameObjectAsItem(player1, PLAYER1);
	turnsManager->AddGameObjectAsItem(player2, PLAYER2);

	//gameObjects.add(testGO);
	//gameObjects.add(testGO2);
	//gameObjects.add(floor);
	gameObjects.add(bomb);
	gameObjects.add(player1);
	gameObjects.add(player2);

	//_app->ui->CreateUI(1234567890, 500, 500, 3.0f);



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

	player1->Update();
	player2->Update();

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

	if (_app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		_app->scene->ChangeCurrentScene(2, 0);
	}

	_app->renderer->camera->MoveCameraWithMouse();

	return true;
}

bool SceneIntro::PostUpdate()
{
	/*rect.x = testGO->GetWorldPosition().x;
	rect.y = testGO->GetWorldPosition().y;*/

	if (_app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		printf("ItemX = %d\n", testGO2->GetScreenPosition().x);
		printf("ItemY = %d\n", testGO2->GetScreenPosition().y);
		printf("MouseX = %d\n", _app->input->GetMouseX());
		printf("MouseY = %d\n", _app->input->GetMouseY());
	}

	bomb->PostUpdate();

	player1->PostUpdate();
	player2->PostUpdate();

	//_app->renderer->AddRectRenderQueue(rect, 255, 0, 0);
	//_app->renderer->DrawCircle(rect.x, rect.y, (int)(std::floor(testGO->rBody->GetRadius() * PIXELS_PER_METERS)), 0,255, 0);

	rect2.x = testGO2->GetWorldPosition().x;
	rect2.y = testGO2->GetWorldPosition().y;

	_app->renderer->DrawCircle(rect2.x, rect2.y, (int)(std::floor(testGO2->rBody->GetRadius() * PIXELS_PER_METERS)), 255, 0, 0);

	/*for (int i = 0; i < gameObjects.count(); i++)
	{
		if (gameObjects[i]->rBody->GetType() == RigidBodyType::STATIC)
		{
			_app->renderer->AddRectRenderQueue({gameObjects[i]->rBody->GetPosition().x-8, gameObjects[i]->rBody->GetPosition().y-8, 32, 32}, 255, 255, 0);
		}
	}*/

	_app->renderer->AddRectRenderQueue({ 0,0,3200,5 }, 255, 0, 0);
	_app->renderer->AddRectRenderQueue({ 1550,0,5,1440 }, 255, 0, 0);
	_app->renderer->AddRectRenderQueue({ 0,0,5,1440 }, 255, 0, 0);

	return true;
}
