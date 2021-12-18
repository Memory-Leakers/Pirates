#pragma once
#include "GameObject.h"
#include "Animation.h"


#define MAX_HEALTH 3
class Player :
    public GameObject
{
public:
	Player(std::string name = "Default", std::string tag = "None", Application* _app = nullptr,int ID = 1);

	~Player();

	void OnCollisionExit(RigidBody* col) override;

	void OnCollisionEnter(RigidBody* col) override;

	void Start() override;

	void PreUpdate() override;

	void Update() override;

	void PostUpdate() override;

	void CleanUp() override;

	void SetAnimations();

private:
	//Depends on the number you choose,you'll get player 1 or two texture
	int ID = 1;

	SDL_Texture* playerTextures[4];

	SDL_Texture* tex_health;

	SDL_Rect player_bounds;

	SDL_Rect player;

	Animation player_Idle;

	Animation player_hit;

	Animation* currentAnimation_player = nullptr;

	Animation anim_health;

public:
	int health = MAX_HEALTH;
	bool hurt = false;
};

