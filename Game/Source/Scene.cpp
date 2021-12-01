#include "Scene.h"

Scene::Scene(Application* app)
{
	_app = app;
}

Scene::~Scene()
{
}

bool Scene::Start()
{
	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update()
{
	return true;
}

bool Scene::PostUpdate()
{
	return true;
}

bool Scene::CleanUp()
{
	for (int i = 0; i < gameObjects.count(); i++)
	{
		gameObjects[i]->CleanUp();
	}

	gameObjects.clearPtr();

	for (int i = 0; i < sceneTextures.count(); i++)
	{
		if(sceneTextures[i])
		{
			_app->textures->Unload(sceneTextures[i]);
		}
	}

	return true;
}

void Scene::DestroyGameObject(GameObject* gameObject)
{
	int index = gameObjects.find(gameObject);

	if (index >= 0)
	{
		gameObjects.del(gameObjects.At(index));
	}
}

void Scene::SetSaveData()
{
}

void Scene::LoadSaveData(pugi::xml_node save)
{
}


