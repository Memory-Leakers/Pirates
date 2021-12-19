#pragma once
#include "Scene.h"
#include "Globals.h"

#include "Player.h"
#include "Bomb.h"

class RigidBody;
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
	Bomb* bombP1 = nullptr;
	Bomb* bombP2 = nullptr;
	Water* water = nullptr;
	GameUI* gameUI = nullptr;

	Player* player1Characters[3] = { nullptr };
	Player* player2Characters[3] = { nullptr };

	iPoint player1Positions[3] = { {230, 180}, {500, 180}, {700, 180} };
	iPoint player2Positions[3] = { {270, 180}, {550, 180}, {750, 180} };

	SDL_Rect rectbgclouds;
	SDL_Texture* bg[4] = { nullptr };

	SDL_Rect rect = { 10,10,10,10 };
	SDL_Rect rect2 = { 10,10,10,10 };
	SDL_Rect rect3 = { 10,10,10,10 };
	RigidBody* body = nullptr;
	RigidBody* body2 = nullptr;

	RigidBody* walls[3] = { nullptr };

	GameObject* testGO = nullptr;
	GameObject* testGO2 = nullptr;
};