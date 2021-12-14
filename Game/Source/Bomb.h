#pragma once
#include "GameObject.h"


enum class BombType
{
    NORMAL = 0,
    BANANA,
    MINI_BANANA,
    UMBRELLA
};

class Bomb :
    public GameObject
{
private:

public:
    Bomb(std::string name = "Default", std::string tag = "None", Application* _app = nullptr, BombType bType = BombType::NORMAL);
    Bomb(GameObject& bomb);

    ~Bomb();

    void OnCollisionEnter(RigidBody* col) override;
    void OnCollisionExit(RigidBody* col) override;
    void OnTriggerEnter(RigidBody* col) override;

    void Start() override;
    void PreUpdate() override;
    void Update() override;
    void PostUpdate() override;

    void CleanUp() override;

   
private:    
    BombType bType = BombType::NORMAL;

public:
    SDL_Rect rect = { 10,10,10,10 };
    int explosionRadius = (int)(std::floor(8 * PIXELS_PER_METERS));
};

