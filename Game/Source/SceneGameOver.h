#pragma once
#include "Scene.h"

enum GAMEOVERBUTTONSTATE
{
	RESTART_BTN,
	MENU_BTN,
	MAX2
};

class BackGround;

class SceneGameOver : public Scene
{
public:
	SceneGameOver(Application* app);

	bool Start() override;

	bool Update() override;

	bool PostUpdate() override;

	void NextBtnState();

	void LastBtnState();

private:
	//BackGround* bg;

	SDL_Texture* buttonsTex[2] = { nullptr };
	SDL_Rect buttonSections[2];

	GAMEOVERBUTTONSTATE btnState;
};
