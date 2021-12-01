#include "SceneMainMenu.h"
#include "SceneGameOver.h"
#include "SceneIntro.h"
#include <time.h>

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "scene";

	scenes[0] = new SceneMainMenu(app);
	scenes[1] = new SceneIntro(app);
	scenes[2] = new SceneGameOver(app);

	playerSettings = PlayerSettings::Instance();
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init(pugi::xml_node& config)
{
	this->config = config;

	return true;
}

bool ModuleScene::Start()
{
	currentScene = scenes[1];

	bool ret = true;

	if (currentScene == nullptr)
	{
		return ret;
	}

	currentScene->Start();


	return ret;
}

UpdateStatus ModuleScene::PreUpdate()
{
	if (currentScene == nullptr)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	if (isChangingScene) isChangingScene = false;

	currentScene->PreUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::Update()
{
	OPTICK_EVENT();
	if (currentScene == nullptr || isChangingScene)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	if (!currentScene->Update())
	{
		return UpdateStatus::UPDATE_STOP;
	}

	return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleScene::PostUpdate()
{
	if (currentScene == nullptr || isChangingScene)
	{
		return UpdateStatus::UPDATE_CONTINUE;
	}

	currentScene->PostUpdate();

	return UpdateStatus::UPDATE_CONTINUE;
}

//CleanUp current scene, change current scene (index), Start current Scene
bool ModuleScene::ChangeCurrentScene(uint index, int frames)
{
	if (isChangingScene)return true;
	isChangingScene = true;
	this->index = index;
	currentScene->CleanUp();
	currentScene = scenes[index];
	currentScene->Start();

	return true;
}

void ModuleScene::GetSaveData(pugi::xml_document& save)
{
	currentScene->SetSaveData();

	pugi::xml_node n = save.child("game_state").child("scene");

	n.child("player").attribute("lifes") = playerSettings->playerLifes;
	n.child("player").attribute("score") = playerSettings->playerScore;

	n.child("player").attribute("x") = currentScene->playerX;
	n.child("player").attribute("y") = currentScene->playerY;

}

void ModuleScene::LoadSaveData(pugi::xml_document& save)
{
	pugi::xml_node n = save.child("game_state").child("scene");

	currentScene->LoadSaveData(n);

	if (!playerSettings->isInit)
	{
		playerSettings->playerLifes = n.child("player").attribute("lifes").as_int();
		playerSettings->playerScore = n.child("player").attribute("score").as_int();
		playerSettings->isInit = true;
	}
}

void ModuleScene::ResetPlayerSettings()
{
	playerSettings->Reset();
}

bool ModuleScene::CleanUp()
{
	for (int i = 0; i < SCENES_NUM; i++)
	{
		if (scenes[i] != nullptr)
		{
			scenes[i]->CleanUp();	//CleanUp all scenes (in case the Application is shut down)
			delete scenes[i];
			scenes[i] = nullptr;
		}
	}

	playerSettings->Release();

	return true;
}
