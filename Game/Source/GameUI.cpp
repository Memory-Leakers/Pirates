#include "GameUI.h"

#include "Application.h"
#include "TurnsManager.h"

GameUI::GameUI(Application* app)
{
	this->_app = app;
	messageCounter = 90;
	currentTurn = 0;
	// Init Textures

	player1Bar = _app->textures->Load("Assets/textures/UI/WeaponSelect/BlueBar.png");
	player2Bar = _app->textures->Load("Assets/textures/UI/WeaponSelect/RedBar.png");
	player1Options = _app->textures->Load("Assets/textures/UI/WeaponSelect/BlueOptions.png");
	player2Options = _app->textures->Load("Assets/textures/UI/WeaponSelect/RedOptions.png");
	optionSelect = _app->textures->Load("Assets/textures/UI/WeaponSelect/SelectOption.png");
	turnMessages = _app->textures->Load("Assets/textures/UI/WeaponSelect/ChangeTurnMessage.png");

}

void GameUI::Update()
{
	//Show change turn Message
	if (showMessage)
	{
		messageCounter--;
		if (messageCounter <= 0)
		{
			messageCounter = 90;
			showMessage = false;
		}
	}
}

void GameUI::PostUpdate()
{
	if (showMessage) _app->renderer->AddTextureRenderQueue(turnMessages, { 0, 150 }, messageSection[currentTurn], 0.5f, 4, 3, 0, SDL_FLIP_NONE, 0);

	switch (currentTurn)
	{
	case 0:
		_app->renderer->AddTextureRenderQueue(player1Bar, { 0,0 }, { 0,0,0,0 }, 0.5f, 3, 1, 0, SDL_FLIP_NONE, 0);
		_app->renderer->AddTextureRenderQueue(player1Options, { 200,5 }, { 0,0,0,0 }, 1.5f, 3, 2, 0, SDL_FLIP_NONE, 0);
		break;
	case 1:
		_app->renderer->AddTextureRenderQueue(player2Bar, { 0,0 }, { 0,0,0,0 }, 0.5f, 3, 1, 0, SDL_FLIP_NONE, 0);
		_app->renderer->AddTextureRenderQueue(player2Options, { 200,5 }, { 0,0,0,0 }, 1.5f, 3, 2, 0, SDL_FLIP_NONE, 0);
		break;
	}

	_app->renderer->AddTextureRenderQueue(optionSelect, optionsPosition[currentOption], { 0,0,0,0 }, 1.5f, 3, 3, 0, SDL_FLIP_NONE, 0);

}

void GameUI::ShowChangeTurnMessage(int currentTurn)
{
	showMessage = true;
	this->currentTurn = currentTurn;
}

void GameUI::ChangeCurrentOption(int option)
{
	currentOption = option;
}
