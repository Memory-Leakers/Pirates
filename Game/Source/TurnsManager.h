#pragma once
#include "GameObject.h"
enum PlayerId
{
	PLAYER1, // = 0
	PLAYER2  // = 1
};

#define THROW_OPTIONS_NUM 4

enum ThrowOptions
{
	BOMB1,
	BOMB2,
	BOMB3,
	CURRENT_ITEM
};

struct Item
{
	GameObject* gameObject;
	bool isSelected = false;
};

class TurnsManager
{
public:
	TurnsManager(Application* app);

	void UpdateGameLogic();

	void AddItem(Item* item, int player);

	void AddGameObjectAsItem(GameObject* g, int player);
	
	int currentPlayer;	// 0 = player1; 1 = player2

	Item* currentItem = nullptr;

	List<Item*> playerItems[2];	// Total Items

	int playerCurrentOption[2] = {3,3};

	bool playerThrowedBomb[2] = { false, false };

	bool playerMovedItem[2] = { false, false };

private:

	bool CheckInteraction();
	void SelectItem();
	void ApplyForces();
	void GetCurrentOption();

	void DrawMouseItemLine();

	void ApplyForceOnOption(fPoint dir);
	void CheckPlayerTurn();

	void ResetCurrentPlayerVariables();


	void CleanUp();

	Application* _app;

	bool canInteract = true;	// Determina si se puede interactuar con los items o no
								// Esta variable debe controlarla un script externo, ya que esta clase no conoce
								// El estado del juego, simplemente controla la logica de los turnos

};

