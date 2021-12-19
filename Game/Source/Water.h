#pragma once
#include "GameObject.h"
class Water : public GameObject
{
public:

	Water(iPoint pos, std::string name, std::string tag, Application* app);

	void Update() override;

	void PostUpdate() override;

};