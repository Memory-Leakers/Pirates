#include "TurnsManager.h"


TurnsManager::TurnsManager(Application* app)
{
	_app = app;
}

/// <summary>
/// Se encarga de gestionar la l�gica de selecci�n de items, aplicado de fuerza sobre estos y los turnos de cada jugador
/// </summary>
void TurnsManager::UpdateGameLogic()
{
	// Fase de Check Interaction
	// Si el juegador no puede interactuar en este frame, se termina la funci�n

	if (!CheckInteraction()) return;

	// Fase de selecci�n de Bomba
	// Donde se mira que opcion ha elegido el jugador
	GetCurrentOption();

	// Fase de selecci�n
	// Donde el jugador activo escoge con qu� item interactuar

	SelectItem();

	// Fase de Dibujo
	// Donde se dibuja la linea entre el objeto seleccionado y el mouse.

	DrawMouseItemLine();

	// Fase de Movimiento
	// Donde se aplican fuerzas en los items seleccionados que han sido soltados

	ApplyForces();

	// Fase de comprobaci�n de turno
	// Donde se determina si el turno actual del jugador ha terminado o no

	CheckPlayerTurn();
}

bool TurnsManager::CheckInteraction()
{
	return true;
}

void TurnsManager::GetCurrentOption()
{
	if (_app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB1;
	}
	if (_app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB2;
	}
	if (_app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB3;
	}
	if (_app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::CURRENT_ITEM;
	}
}

void TurnsManager::SelectItem()
{
	for (int i = 0; i < playerItems[currentPlayer].count(); i++)
	{
		if (playerItems[currentPlayer][i] == nullptr) return;
		
		float itemX = playerItems[currentPlayer][i]->gameObject->rBody->GetPosition().x;
		float itemY = playerItems[currentPlayer][i]->gameObject->rBody->GetPosition().y;

		fPoint mouseDistance = { (float)abs(_app->input->GetMouseX() - itemX),
								(float)abs(_app->input->GetMouseY() - itemY) };

		float mouseModule = mouseDistance.Module();

		if (_app->input->GetMouseButton(1) == KEY_DOWN && mouseModule < 20 ) //&& // Encontrar si el mouse est� dentro de la hitbox del item) 
		{													// Se puede hacer usando la posici�n del GameObject y a�adiendole
															// Un area determinada (20 pixeles por ejemplo)
															// Si el rat�n est� dentro de ese area, se selecciona el item
															// Si Hemos hecho click en un item que nos pertenece
															// Sin embargo de esta manera dos items del mismo jugador NO puedes compartir espacio
															// Porque no sabriamos que espacio estamos seleccionando (estariamos seleccionando dos espacios a la vez)

			if (currentItem != nullptr) currentItem->isSelected = false;	// El Item que estaba seleccionado pasa a no estarlo

			playerItems[currentPlayer][i]->isSelected = true;	// Seleccionamos nuestro item

			currentItem = playerItems[currentPlayer][i];	// Lo guardamos como item seleccionado actualmente
		}
	}
}

void TurnsManager::DrawMouseItemLine()
{
	if (currentItem == nullptr) return;	// Si no hay nignun objeto seleccionado no ejecutamos

	if (_app->input->GetMouseButton(1) == KEY_REPEAT)	// Si estamos manteniendo el boton del rat�n, dibujamos la linea
	{
		iPoint itemPos = { (int)currentItem->gameObject->rBody->GetPosition().x, (int)currentItem->gameObject->rBody->GetPosition().y };
		iPoint mousePos;
		mousePos.x = _app->input->GetMouseX();
		mousePos.y = _app->input->GetMouseY();

		_app->renderer->DrawLine(mousePos.x, mousePos.y, itemPos.x, itemPos.y, 255, 0, 0);
	}
}

void TurnsManager::ApplyForces()
{
	if (currentItem == nullptr) return;

	if (_app->input->GetMouseButton(1) == KEY_UP) // Si soltamos el boton del raton
	{
		fPoint itemPos = currentItem->gameObject->rBody->GetPosition();
		iPoint mousePos;
		mousePos.x = _app->input->GetMouseX();
		mousePos.y = _app->input->GetMouseX();

		// Si la posici�n del rat�n est� cerca de la del item, no aplicamos fuerza y cancelamos el lanzamiento
		if (abs(mousePos.x - itemPos.x) < 10 && abs(mousePos.y - itemPos.y) < 10)
		{
			currentItem = nullptr;
		}

		fPoint dir = { (float)(itemPos.x - mousePos.x), (float)(itemPos.y - mousePos.y) };	// El vector que determina hacia donde estamos apuntando

		float throwForce = 3.0f;

		dir *= throwForce;

		ApplyForceOnOption(dir); // Aplicamos la fuerza usando el vector que hemos determinado. 
	}
}

void TurnsManager::ApplyForceOnOption(fPoint dir)
{
	switch (playerCurrentOption[currentPlayer])
	{
	case 0:
		// Create bomb of type "Bomb 1"
		// Apply force to created Bomb
		// Example:

		//Bomb bomb = new Bomb(itemPos, 0)

		//bomb.AddForceToCenter(dir)

		//Where Bomb(iPoint pos, int bombType)

		playerThrowedBomb[currentPlayer] = true;
		break;
	case 1:
		// Create bomb of type "Bomb 2"
		// Apply force to created Bomb
		// Example:

		//Bomb bomb = new Bomb(itemPos, 1)

		//bomb.AddForceToCenter(dir)

		//Where Bomb(iPoint pos, int bombType)

		playerThrowedBomb[currentPlayer] = true;
		break;
	case 2:
		// Create bomb of type "Bomb 3"
		// Apply force to created Bomb
		// Example:

		//Bomb bomb = new Bomb(itemPos, 2)

		//bomb.AddForceToCenter(dir)

		//Where Bomb(iPoint pos, int bombType)

		playerThrowedBomb[currentPlayer] = true;
		break;
	case 3:
		if (!playerMovedItem[currentPlayer])
		currentItem->gameObject->rBody->AddForceToCenter(dir);	// Aplicamos fuerza en la direcci�n que hemos determinado

		playerMovedItem[currentPlayer] = true;
		break;
	}
}

void TurnsManager::CheckPlayerTurn()
{
	// Next turn if player throwed a bomb or pressed Space
	if (playerThrowedBomb[currentPlayer] || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		// Reset variables from current player
		ResetCurrentPlayerVariables();

		// Change to the other player
		currentPlayer = currentPlayer == 0 ? 1 : 0;

		printf("Turno player %d", currentPlayer);
	}
}

void TurnsManager::ResetCurrentPlayerVariables()
{
	playerMovedItem[currentPlayer] = false;
	playerThrowedBomb[currentPlayer] = false;
	currentItem = nullptr;
}


void TurnsManager::AddItem(Item* item, int player)
{
	if (player > 1) return;

	playerItems[player].add(item);
}

void TurnsManager::AddGameObjectAsItem(GameObject* g, int player)
{
	if (player > 1) return;

	Item* item = new Item();
	item->gameObject = g;

	playerItems[player].add(item);
}

void TurnsManager::CleanUp()
{
	// delete all items
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < playerItems[i].count(); j++)
		{
			if (playerItems[i][j] != nullptr)
			{
				delete playerItems[i][j];
				playerItems[i][j] = nullptr;
			}
		}
	}
}