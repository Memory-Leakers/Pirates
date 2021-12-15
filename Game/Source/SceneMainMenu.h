#pragma once
#include "Scene.h"

enum BUTTONSTATE 
{
	PLAY_BTN,
	EXIT_BTN,
	MAX
};


class BackGround;
class SceneMainMenu : public Scene
{
public:
	SceneMainMenu(Application* app);

	bool Start() override;

	bool Update() override;

	bool PostUpdate() override;

	void NextBtnState();

	void LastBtnState();

private:
	iPoint arrowPos;
	//BackGround* bg;

	SDL_Texture* titleTex;

	SDL_Texture* buttonsTex[2];
	SDL_Rect buttonSections[2];

	BUTTONSTATE btnState;
};