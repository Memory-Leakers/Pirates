#include "Application.h"

Application::Application()
{
	renderer = new ModuleRender(this);
	window = new ModuleWindow(this);
	textures = new ModuleTextures(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene = new ModuleScene(this);
	ui = new ModuleUI(this);
	map = new ModuleMap(this);
	debug = new ModuleDebug(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(input);
	AddModule(debug);

	// Game Logic
	AddModule(scene);

	AddModule(textures);	
	AddModule(audio);
	AddModule(ui);
	AddModule(map);
	
	//Render
	AddModule(renderer);
}

Application::~Application()
{
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL)
	{
		delete item->data;
		item = item->prev;
	}
}

bool Application::Init()
{
	bool ret = true;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configF);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title = configApp.child("title").child_value();
		organization = configApp.child("organization").child_value();
	}

	// Call Init() in all modules
	p2List_item<Module*>* item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		ret = item->data->Init(config.child(item->data->name.c_str()));
	
		item = item->next;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.getFirst();

	while(item != NULL && ret == true)
	{
		if(item->data->IsEnabled())
			ret = item->data->Start();
		item = item->next;
	}

	LoadGameRequest();

	return ret;
}

// Call PreUpdate, Update and PostUpdate on all modules
UpdateStatus Application::Update()
{
	if (isExiting) return UPDATE_STOP;
	OPTICK_EVENT();
	UpdateStatus ret = UPDATE_CONTINUE;
	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PreUpdate();
		item = item->next;
	}

	item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->Update();
		item = item->next;
	}

	item = list_modules.getFirst();

	while (item != NULL && ret == UPDATE_CONTINUE)
	{
		if (item->data->IsEnabled())
			ret = item->data->PostUpdate();
		item = item->next;
	}

	//// L02: DONE 1: This is a good place to call Load / Save methods
	if (saveGameRequested == true) SaveGame();
	if (loadGameRequested == true) LoadGame();

	// Update globalTime
	globalTime.Update();

	deltaTime = globalTime.getDeltaTime();

	if (deltaTime <= (1.0f / fps))
	{
		sleepTime = ((1.0f / fps) - deltaTime) * 1000;
		Sleep(sleepTime);
	}

	globalTime.Reset();

	ShowTime();

	return ret;
}

void Application::ShowTime()
{
	OPTICK_EVENT();
	LOG("Finish Update");
}

bool Application::CleanUp()
{
	bool ret = true;
	p2List_item<Module*>* item = list_modules.getLast();

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

pugi::xml_node Application::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description())
	else ret = configFile.child("config");

	return ret;
}

void Application::ExitGame()
{
	SaveGameRequest();
	isExiting = true;
}

void Application::AddModule(Module* mod)
{
	list_modules.add(mod);
}

void Application::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void Application::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

// ---------------------------------------
const char* Application::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
const char* Application::GetOrganization() const
{
	return organization.c_str();
}

// ---------------------------------------
// then call all the modules to load themselves
bool Application::LoadGame()
{
	bool ret = false;

	pugi::xml_document saveGame;
	pugi::xml_parse_result result = saveGame.load_file(SAVE_STATE_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description())
	else ret = saveGame.child("game_state");


	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL)
	{
		item->data->LoadSaveData(saveGame);
		item = item->next;
	}

	loadGameRequested = false;

	return ret;
}

// L02: TODO 7: Implement the xml save method for current state
bool Application::SaveGame() const
{
	bool ret = true;

	pugi::xml_document saveGame;
	pugi::xml_parse_result result = saveGame.load_file(SAVE_STATE_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description())
	else ret = saveGame.child("game_state");

	p2List_item<Module*>* item = list_modules.getFirst();

	while (item != NULL)
	{
		item->data->GetSaveData(saveGame);
		item = item->next;
	}

	saveGame.save_file(SAVE_STATE_FILENAME);

	saveGameRequested = false;

	return ret;
}