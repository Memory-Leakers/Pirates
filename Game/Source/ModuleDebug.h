#pragma once
#include "Module.h"

enum class DebugMode
{
	NOT_DEBUG,
	DEBUG,
	DEBUG_FPS,
	DEBUG_FORCE
};

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);

	~ModuleDebug();

	bool Start() override;

	UpdateStatus PreUpdate() override;

	UpdateStatus Update() override;

	UpdateStatus PostUpdate() override;

	void ResetDebug();

	bool CleanUp() override;

private:
	void ChangeDebugMode();

	void DebugFPS();

	void DebugForce();

public:
	DebugMode debugMode = DebugMode::NOT_DEBUG;
	bool gravityOn = true;
	bool buoyancyOn = true;
	bool hydrioDragOn = true;
	bool aeroDragOn = true;
	bool clippingOn = true;
	bool frictioOn = true;
	int fps = 60;
	int force = 25;

	int fpsUIPos = 0;
	int forceUIPos = 0;

private:
	SDL_Texture* debugPanelTex = nullptr;
	SDL_Texture* selectTex = nullptr;
	SDL_Texture* arrowTex = nullptr;

	iPoint gravityGrid = { 314,206 };
};