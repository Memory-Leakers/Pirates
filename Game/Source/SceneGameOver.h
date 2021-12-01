#pragma once
#include "Scene.h"
class BackGround;
class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start() override;

	bool Update() override;
	bool PostUpdate() override;

	void SetSaveData() override;

private:
	iPoint arrowPos;
	SDL_Texture* mainMenu;
	SDL_Texture* arrow;

	iPoint playerStartPos = { 40, 567 };
};
