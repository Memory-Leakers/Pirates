#pragma once
#include "Scene.h"
#include "Globals.h"

#include "Bomb.h"

class PhysCore;
class RigidBody;
class TurnsManager;

class SceneIntro : public Scene
{
public:
	SceneIntro(Application* app);
	~SceneIntro();

	bool Start();
	void InitScene() override;
	bool Update();
	bool PostUpdate();
	bool CleanUp() override;

private:
	

public:
	Bomb* bomb;
	SDL_Rect rect = { 10,10,10,10 };
	SDL_Rect rect2 = { 10,10,10,10 };
	SDL_Rect rect3 = { 10,10,10,10 };
	RigidBody* body;
	RigidBody* body2;

	RigidBody* walls[3];
	
	PhysCore* world;

	GameObject* testGO = nullptr;
	GameObject* testGO2 = nullptr;

	TurnsManager* turnsManager;

};
