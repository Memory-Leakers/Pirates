#pragma once
#include "Module.h"

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);

	~ModuleDebug();

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	bool CleanUp() override;

private:
	void ChangeDebugMode();

public:
	bool debugMode = false;
	bool gravityOn = true;
	bool buoyancyOn = true;
	bool hydrioDragOn = true;
	bool aeroDragOn = true;
	bool clippingOn = true;
	bool frictioOn = true;

private:
	SDL_Texture* debugPanelTex;
	SDL_Texture* selectTex;

	iPoint gravityGrid = { 314,206 };
};