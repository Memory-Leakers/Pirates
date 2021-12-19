#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Timer.h"

#define bounceSize 4

enum class BombType
{
    NORMAL = 0,
    BANANA,
    UMBRELLA
};

class Bomb :
    public GameObject
{
private:
    void SetAnimations();
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

    void setType(BombType type);

private:
    BombType bType = BombType::NORMAL;
    bool exploded = false;
    int bounceCount = bounceSize;

    SDL_Texture* tex_normal = nullptr;
    SDL_Texture* tex_banana = nullptr;
    SDL_Texture* tex_umbrella = nullptr;
    SDL_Texture* tex_explosion = nullptr;

    Animation anim_normal;
    Animation anim_banana;
    Animation anim_umbrella;
    Animation anim_explosion;

public:
    SDL_Rect bounds = { 10,10,10,10 };

    SDL_Rect rect;

    int explosionRadius = (int)(std::floor(6.0 * PIXELS_PER_METERS));
    int bombRadius = (int)(std::floor(0.8 * PIXELS_PER_METERS));
};
