#ifndef __MODULESCENE_H__
#define __MODULESCENE_H__

#include "Module.h"
#include "Globals.h"
#include "PlayerSettings.h"

#define SCENES_NUM 9

class Scene;
class PhysCore;

enum SCENES
{
	SCENE_MENU,
	SCENE_GAME,
	SCENE_GAME_OVER,
	TOTAL_SCENES
};

class ModuleScene : public Module
{
public:
	// Constructor
	ModuleScene(Application* app, bool start_enabled = true);

	// Destructor
	~ModuleScene();

	bool Init(pugi::xml_node&);

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start();

	UpdateStatus PreUpdate() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	UpdateStatus Update();

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	UpdateStatus PostUpdate();

	bool ChangeCurrentScene(uint index, int frames);

	void GetSaveData(pugi::xml_document& save) override;

	void LoadSaveData(pugi::xml_document& save) override;

	void ResetPlayerSettings();

	bool CleanUp();

public:

	SCENES index = SCENE_MENU;

	Scene* currentScene = nullptr;

	Scene* scenes[SCENES_NUM] = { nullptr };

	bool isChangingScene = false;

	int playerX, playerY;

	PlayerSettings* playerSettings = nullptr;

	bool DEBUGMODE = false;

private:
	//int newScene;
};

#endif // __MODULESCENE_H__

