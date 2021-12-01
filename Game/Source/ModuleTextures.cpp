#include "Application.h"

#include "SDL_image/include/SDL_image.h"

//#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ModuleTextures::ModuleTextures(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "textures";
}

// Destructor
ModuleTextures::~ModuleTextures()
{}

// Called before render is available
bool ModuleTextures::Init(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;

	this->config = config;

	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before quitting
bool ModuleTextures::CleanUp()
{
	LOG("Freeing textures and Image library");

	p2List_item<SDL_Texture*>* item = textures.getFirst();

	while(item != NULL)
	{
		SDL_DestroyTexture(item->data);
		item = item->next;
	}

	textures.clear();

	texturePath.clear();

	IMG_Quit();
	return true;
}

bool ModuleTextures::CleanUpTextures()
{
	LOG("Freeing textures and Image library");

	p2List_item<SDL_Texture*>* item = textures.getFirst();

	while (item != NULL)
	{
		SDL_DestroyTexture(item->data);
		item = item->next;
	}

	textures.clear();

	texturePath.clear();

	return true;
}

 //Load new texture from file path
SDL_Texture* ModuleTextures::Load(std::string path, bool isName)
{
	if(isName)
	{
		path = config.child(path.c_str()).attribute("path").as_string();
	}

	std::map<std::string, SDL_Texture*>::iterator it;
	it = texturePath.find(path);

	if (it != texturePath.end())
	{
		return texturePath.find(path)->second;
	}

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path.c_str());

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(App->renderer->renderer, surface);

		if(texture == NULL)
		{
			LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			textures.add(texture);
		}

		SDL_FreeSurface(surface);
	}

	texturePath.insert(std::pair<std::string, SDL_Texture*>(path, texture));

	return texture;
}

// Free texture from memory
void ModuleTextures::Unload(SDL_Texture* texture)
{
	p2List_item<SDL_Texture*>* item = textures.getFirst();

	while(item != NULL)
	{
		if(item->data == texture)
		{
			SDL_DestroyTexture(item->data);
			textures.del(item);
			break;
		}
		item = item->next;
	}
}
