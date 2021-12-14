#include "TurnsManager.h"


TurnsManager::TurnsManager(Application* app)
{
	_app = app;
}

/// <summary>
/// Se encarga de gestionar la lógica de selección de items, aplicado de fuerza sobre estos y los turnos de cada jugador
/// </summary>
void TurnsManager::UpdateGameLogic()
{
	// Comprobar que el objeto tirado (si lo hay) aún está en movimiento
	if (throwedGameObj != nullptr)
	{
		if (throwedGameObj->rBody->GetLinearVelocity().Module() < 0.5f)
		{
			throwedGameObj = nullptr;
		}
	}

	// Fase de Check Interaction
	// Si el juegador no puede interactuar en este frame, se termina la función

	if (!CheckInteraction()) return;

	// Fase de selección de Bomba
	// Donde se mira que opcion ha elegido el jugador
	GetCurrentOption();

	// Fase de selección
	// Donde el jugador activo escoge con qué item interactuar

	SelectItem();

	// Fase de Dibujo
	// Donde se dibuja la linea entre el objeto seleccionado y el mouse.

	DrawMouseItemLine();

	// Fase de Movimiento
	// Donde se aplican fuerzas en los items seleccionados que han sido soltados

	ApplyForces();

	// Fase de comprobación de turno
	// Donde se determina si el turno actual del jugador ha terminado o no

	CheckPlayerTurn();
}

bool TurnsManager::CheckInteraction()
{
	return true;
}

void TurnsManager::GetCurrentOption()
{
	if (_app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB1;
	}
	if (_app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB2;
	}
	if (_app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::BOMB3;
	}
	if (_app->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		playerCurrentOption[currentPlayer] = ThrowOptions::CURRENT_ITEM;
	}
}

void TurnsManager::SelectItem()
{
	for (int i = 0; i < playerItems[currentPlayer].count(); i++)
	{
		if (playerItems[currentPlayer][i] == nullptr) return;

		//printf("%f\n", GetMouseModule(playerItems[currentPlayer][i]));

		if (_app->input->GetMouseButton(1) == KEY_DOWN && GetMouseModule(playerItems[currentPlayer][i]) < 20 ) //&& // Encontrar si el mouse está dentro de la hitbox del item) 
		{													// Se puede hacer usando la posición del GameObject y añadiendole
															// Un area determinada (20 pixeles por ejemplo)
															// Si el ratón está dentro de ese area, se selecciona el item
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

	if (_app->input->GetMouseButton(1) == KEY_REPEAT)	// Si estamos manteniendo el boton del ratón, dibujamos la linea
	{
		iPoint itemPos = { (int)currentItem->gameObject->GetScreenPosition().x, (int)currentItem->gameObject->GetScreenPosition().y };
		mousePos.x = _app->input->GetMouseX();
		mousePos.y = _app->input->GetMouseY();

		// Si la linea es muy larga, no dejamos que se haga más grande
		float mouseMaxModule = 100 / GetMouseModule(currentItem);
		// Si la division del modulo por el rango maximo es menor que uno, significa que el modulo es mayor que el rango...
		if (mouseMaxModule < 1)
		{
			//Obtener posicion de mouse respecto a la base del item
			iPoint mousePosOnItem = mousePos - itemPos;
			//Aplicar modulo maximo a esta base
			mousePosOnItem *= mouseMaxModule;
			//Transformar posicion de mouse a la base canónica de nuevo
			mousePos = mousePosOnItem + itemPos;
		}

		//_app->renderer->DrawLine(mousePos.x, mousePos.y, itemPos.x, itemPos.y, 255, 0, 0);
		_app->renderer->AddLineRenderQueue({ mousePos.x, mousePos.y }, { itemPos.x, itemPos.y }, 255, 0, 0, 255, 2, 100);
	}
}

void TurnsManager::ApplyForces()
{
	if (currentItem == nullptr) return;


	if (_app->input->GetMouseButton(1) == KEY_UP) // Si soltamos el boton del raton
	{
		iPoint itemPos = currentItem->gameObject->GetScreenPosition();

		// Si la posición del ratón está cerca de la del item, no aplicamos fuerza y cancelamos el lanzamiento
		if (GetMouseModule(currentItem) < 50)
		{
			currentItem = nullptr;
			return;
		}

		// Aplicamos fuerza
		fPoint dir = { (float)(itemPos.x - mousePos.x), (float)(itemPos.y - mousePos.y) };	// El vector que determina hacia donde estamos apuntando

		float throwForce = 10.0f;

		dir *= throwForce;

		ApplyForceOnOption(dir); // Aplicamos la fuerza usando el vector que hemos determinado. 

		currentItem = nullptr;
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
		if (playerMovedItem[currentPlayer]) return;
	
		currentItem->gameObject->rBody->AddForceToCenter(dir);	// Aplicamos fuerza en la dirección que hemos determinado

		throwedGameObj = currentItem->gameObject;

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
	throwedGameObj = nullptr;
}

float TurnsManager::GetMouseModule(Item* item)
{
	float itemX = item->gameObject->GetScreenPosition().x;
	float itemY = item->gameObject->GetScreenPosition().y;

	fPoint mouseDistance = { (float)abs(_app->input->GetMouseX() - itemX),
							(float)abs(_app->input->GetMouseY() - itemY) };

	float mouseModule = mouseDistance.Module();
	return mouseModule;
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