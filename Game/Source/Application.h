#pragma once

#include "p2List.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleScene.h"
#include "ModuleUI.h"
#include "ModuleMap.h"

#include "Timer.h"
#include "Optick/include/optick.h"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

#define FPS 60
#define FRAME_TIME (1.0/FPS)

class Application
{
public:
	ModuleRender* renderer;
	ModuleWindow* window;
	ModuleTextures* textures;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleScene* scene;
	ModuleUI* ui;
	ModuleMap* map;

	Timer globalTime;

	bool isDebug = false;

	std::string title;
	std::string organization;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	pugi::xml_node config;
	pugi::xml_node configApp;
	pugi::xml_document configF;
	
private:
	p2List<Module*> list_modules;

public:
	Application();
	~Application();

	bool Init();
	UpdateStatus Update();
	bool CleanUp();

	void LoadGameRequest();
	void SaveGameRequest() const;

	void ShowTime();

	const char* GetTitle() const;
	const char* GetOrganization() const;

	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	void ExitGame();

	bool FullScreenDesktop = true;

private:

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

	float deltaTime, sleepTime;

	void AddModule(Module* mod);

	bool isExiting = false;
};