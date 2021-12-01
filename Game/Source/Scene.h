#ifndef _SCENE_H_
#define _SCENE_H_

#include "Application.h"

#include "GameObject.h"

#include "List.h"

class Scene
{
protected:

	int ID;

	Application* _app;

	List<GameObject*> gameObjects;

	List<SDL_Texture*> sceneTextures;

public:
	// Constructor
	Scene(Application* app);

	// Destructor
	~Scene();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update();

	virtual bool PostUpdate();

	virtual bool CleanUp();

	void DestroyGameObject(GameObject* gameObject);

	virtual void SetSaveData();

	virtual void LoadSaveData(pugi::xml_node save);

	int getID()
	{
		return ID;
	}

	int lastID = 0;

	int playerX;
	int playerY;
};


#endif //_SCENE_H_

