#include "Application.h"
#include "PhysCore.h"

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

	if(!debugMode)   return UpdateStatus::UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		gravityOn = !gravityOn;
		App->scene->world->gravityOn = gravityOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		buoyancyOn = !buoyancyOn;
		App->scene->world->buoyancyOn = buoyancyOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		hydrioDragOn = !hydrioDragOn;
		App->scene->world->hydrioDragOn = hydrioDragOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		aeroDragOn = !aeroDragOn;
		App->scene->world->aeroDragOn = aeroDragOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		clippingOn = !clippingOn;
		App->scene->world->clippingOn = clippingOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		frictioOn = !frictioOn;
		App->scene->world->frictioOn = frictioOn;
	}
	else if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		//fps
	}
	else if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		//force
	}
    return UpdateStatus::UPDATE_CONTINUE;
}

UpdateStatus ModuleDebug::PostUpdate()
{
	if (debugMode)
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
	}

    return UpdateStatus::UPDATE_CONTINUE;
}

bool ModuleDebug::CleanUp()
{
    return false;
}

void ModuleDebug::ChangeDebugMode()
{
	debugMode = !debugMode;
}
