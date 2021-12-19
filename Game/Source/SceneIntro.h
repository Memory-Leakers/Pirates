#pragma once
#include "Scene.h"
#include "Globals.h"

#include "Player.h"
#include "Bomb.h"

class PhysCore;
class RigidBody;
class TurnsManager;
class Water;
class GameUI;

class SceneIntro : public Scene
{
public:
	SceneIntro(Application* app);
	~SceneIntro();

	bool Start();
	void InitScene() override;
	bool PreUpdate() override;
	bool Update();
	bool PostUpdate();
	bool CleanUp() override;

public:
	Bomb* bombP1;
	Bomb* bombP2;
	Player* player1;
	Player* player2;
	Water* water;
	GameUI* gameUI;

	SDL_Texture* bg[4];

	SDL_Rect rect = { 10,10,10,10 };
	SDL_Rect rect2 = { 10,10,10,10 };
	SDL_Rect rect3 = { 10,10,10,10 };
	RigidBody* body;
	RigidBody* body2;

	RigidBody* walls[3];

	GameObject* testGO = nullptr;
	GameObject* testGO2 = nullptr;

	TurnsManager* turnsManager;
};
