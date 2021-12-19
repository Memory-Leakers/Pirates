#pragma once

#include "SDL/include/SDL.h"
#include "Point.h"

class Application;

class GameUI
{
public:
	GameUI(Application* app);

	void Update();
	void PostUpdate();
	void ShowChangeTurnMessage(int currentTurn);
	void ChangeCurrentOption(int option);

private:
	Application* _app = nullptr;

	int currentTurn = 0;

	SDL_Texture* player1Bar = nullptr;
	SDL_Texture* player2Bar = nullptr;
	SDL_Texture* player1Options = nullptr;
	SDL_Texture* player2Options = nullptr;
	SDL_Texture* optionSelect = nullptr;
	SDL_Texture* turnMessages = nullptr;

	SDL_Rect messageSection[2] = { { 0,0, 1280, 128 }, { 0,128,1280, 128 } };
	iPoint optionsPosition[4] = { {200, 5}, {272, 5}, {344, 5}, {416, 5} };

	int messageCounter = 90;

	bool showMessage = false;

	int currentOption = 3;

};

