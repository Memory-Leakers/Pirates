#include "Application.h"
#include "PhysCore.h"
#include "Scene.h"
#include "TurnsManager.h"

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    name = "debug";
}

ModuleDebug::~ModuleDebug()
{
}

bool ModuleDebug::Start()
{
	debugPanelTex = App->textures->Load("Assets/textures/UI/Debug/debugPanel.png");
	selectTex = App->textures->Load("Assets/textures/UI/Debug/select.png");
	arrowTex = App->textures->Load("Assets/textures/UI/Debug/arrow.png");
	
	fps = App->fps;

	return true;
}

UpdateStatus ModuleDebug::PreUpdate()
{
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleDebug::Update()
{
	if (App->scene->index != SCENES::SCENE_GAME)  return UpdateStatus::UPDATE_CONTINUE;

	if(App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RCTRL) == KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		{
			ChangeDebugMode();
		}
	}

	if (debugMode == DebugMode::NOT_DEBUG)   return UpdateStatus::UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		gravityOn = !gravityOn;
		App->scene->currentScene->world->gravityOn = gravityOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		buoyancyOn = !buoyancyOn;
		App->scene->currentScene->world->buoyancyOn = buoyancyOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		hydrioDragOn = !hydrioDragOn;
		App->scene->currentScene->world->hydrioDragOn = hydrioDragOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		aeroDragOn = !aeroDragOn;
		App->scene->currentScene->world->aeroDragOn = aeroDragOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		clippingOn = !clippingOn;
		App->scene->currentScene->world->clippingOn = clippingOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		frictioOn = !frictioOn;
		App->scene->currentScene->world->frictioOn = frictioOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		//fps
		if (debugMode != DebugMode::DEBUG_FPS) debugMode = DebugMode::DEBUG_FPS;
		else debugMode = DebugMode::DEBUG;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		//fps
		if (debugMode != DebugMode::DEBUG_FORCE) debugMode = DebugMode::DEBUG_FORCE;
		else debugMode = DebugMode::DEBUG;
	}

	if (debugMode == DebugMode::DEBUG_FPS) DebugFPS();

	else if (debugMode == DebugMode::DEBUG_FORCE) DebugForce();

    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleDebug::PostUpdate()
{
	if ((int)debugMode > 0)
	{
		// debug panel
		App->renderer->AddTextureRenderQueue(debugPanelTex, { 0,0 }, { 0,0,0,0 },0.5,4,0,0,SDL_RendererFlip::SDL_FLIP_NONE,0);

		// selections
		if (gravityOn) App->renderer->AddTextureRenderQueue(selectTex, { 157, 103}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
		if (buoyancyOn) App->renderer->AddTextureRenderQueue(selectTex, { 479, 103}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
		if (hydrioDragOn) App->renderer->AddTextureRenderQueue(selectTex, { 157, 160}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
		if (aeroDragOn) App->renderer->AddTextureRenderQueue(selectTex, { 479, 160}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
		if (clippingOn) App->renderer->AddTextureRenderQueue(selectTex, { 157, 219}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
		if (frictioOn) App->renderer->AddTextureRenderQueue(selectTex, { 479, 219}, { 0,0,0,0 }, 0.5, 4, 1, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
	
		switch (debugMode)
		{
			case DebugMode::DEBUG_FPS:
				App->renderer->AddTextureRenderQueue(arrowTex, { 283, 261 }, { 0,0,0,0 }, 0.5, 4, 2, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
				break;
			case DebugMode::DEBUG_FORCE:
				App->renderer->AddTextureRenderQueue(arrowTex, { 283, 305 }, { 0,0,0,0 }, 0.5, 4, 2, 0, SDL_RendererFlip::SDL_FLIP_NONE, 0);
				break;
		}
	}

    return UpdateStatus::UPDATE_CONTINUE;
}

void ModuleDebug::ResetDebug()
{
	if (App->scene->currentScene->turnsManager == nullptr) return;
	debugMode = DebugMode::NOT_DEBUG;
	gravityOn = true;
	buoyancyOn = true;
	hydrioDragOn = true;
	aeroDragOn = true;
	clippingOn = true;
	frictioOn = true;
	fps = 60;
	force = 25;
	App->fps = fps;
	App->scene->currentScene->turnsManager->throwForce = force;
}

bool ModuleDebug::CleanUp()
{
    return false;
}

void ModuleDebug::ChangeDebugMode()
{
	if (debugMode != DebugMode::NOT_DEBUG) debugMode = DebugMode::NOT_DEBUG;
	else debugMode = DebugMode::DEBUG;

	if ((int)debugMode > 0)
	{
		fpsUIPos = App->ui->CreateUI(fps, 306, 273, 0.25f, 4, 1, false, 0, { 0, 0 }, 0);
		forceUIPos = App->ui->CreateUI(force, 306, 316, 0.25f, 4, 1, false, 0, { 0, 0 }, 0);
	}
	else
	{
		App->ui->DestroyUI(fpsUIPos);
		App->ui->DestroyUI(forceUIPos);
	}
}

void ModuleDebug::DebugFPS()
{
	App->ui->DestroyUI(fpsUIPos);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (fps > 15)
		{
			fps--;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (fps < 120)
		{
			fps++;
		}
	}

	fpsUIPos = App->ui->CreateUI(fps, 306, 273, 0.25f, 4, 1, false, 0, { 0, 0 }, 0);

	App->fps = fps;
}

void ModuleDebug::DebugForce()
{
	App->ui->DestroyUI(forceUIPos);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (force > 1)
		{
			force--;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (force < 300)
		{
			force++;
		}
	}

	forceUIPos = App->ui->CreateUI(force, 306, 316, 0.25f, 4, 1, false, 0, { 0, 0 }, 0);

	App->scene->currentScene->turnsManager->throwForce = force;
}