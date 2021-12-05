#pragma once
#include "Scene.h"
#include "Globals.h"

class PhysCore;
class RigidBody;
class TurnsManager;

class SceneIntro : public Scene
{
public:
	SceneIntro(Application* app);
	~SceneIntro();

	bool Start();
	bool Update();
	bool PostUpdate();
	bool CleanUp() override;

public:

	SDL_Rect rect = { 10,10,10,10 };
	SDL_Rect rect2 = { 10,10,10,10 };
	RigidBody* body;
	RigidBody* body2;
	
	PhysCore* world;

	GameObject* testGO = nullptr;
	GameObject* testGO2 = nullptr;

	TurnsManager* turnsManager;
};
