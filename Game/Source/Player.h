#pragma once
#include "GameObject.h"
#include "Animation.h"

class Player :
    public GameObject
{
public:
	Player(std::string name = "Default", std::string tag = "None", Application* _app = nullptr);

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

	iPoint position { 0,0 };

	SDL_Texture* player1_idle_tex = nullptr;

	SDL_Texture* player2_idle_tex = nullptr;

	SDL_Texture* player1_hit_tex = nullptr;

	SDL_Texture* player2_hit_tex = nullptr;

	SDL_Rect player_bounds;

	SDL_Rect player;

	Animation player_Idle;

	Animation player_hit;

	Animation* currentAnimation_player1 = nullptr;

};

