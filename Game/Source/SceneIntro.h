#pragma once
#include "Scene.h"
#include "Animation.h"
#include "p2DynArray.h"
#include "Globals.h"

class PhysCore;
class RigidBody;

#define BOUNCER_TIME 200

struct Bouncer
{
	Bouncer() : body(NULL), texture(NULL), hit_timer(0), fx(0)
	{}

	PhysBody* body;
	SDL_Texture* texture;
	Uint32 hit_timer;
	uint fx;
};

enum lightTypes
{
	tiny,
	medium,
	big
};

class ModuleSceneIntro;

//struct Light
//{
//	Light() : body(NULL), texture(NULL), on(false), fx(0)
//	{}
//
//	Light(ModuleSceneIntro* physics, int x, int y, lightTypes type);
//
//	lightTypes type;
//	PhysBody* body;
//	SDL_Texture* texture;
//	bool on;
//	uint fx;
//	int x, y;
//};

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

	//Borrar
	SDL_Rect plant; 
	SDL_Rect ground;
	SDL_Rect player;
	iPoint playerCenter;
	
	//Borrar
	SDL_Texture* graphics;
	PhysBody* background;

	Bouncer bouncer1;
	Bouncer bouncer2;

	Bouncer side_bouncer1;
	Bouncer side_bouncer2;

	SDL_Texture* tex_light_tiny;
	SDL_Texture* tex_light_medium;
	SDL_Texture* tex_light_big;
	
	uint fx_light_tiny;
	uint fx_light_medium;
	uint fx_light_big;

	//p2DynArray<Light> lights;

	PhysBody* player_lose;
	uint player_lose_fx;

	PhysCore* world;
};
