#pragma once
#include "Scene.h"

class BackGround;
class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

private:
	iPoint arrowPos;
	//BackGround* bg;

	//SDL_Texture* mainMenu;

	SDL_Texture* startBtn;

	SDL_Texture* exitBtn;

	//SDL_Texture* arrow;

	//SDL_Texture* logo;
};