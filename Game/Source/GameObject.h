#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <string>
#include "Point.h"
#include "Application.h"
#include "RigidBody.h"
struct SDL_Texture;

#define MAX_GAMEOBJECT_TEXTURES 6

class GameObject 
{
public:

	GameObject();

	GameObject(std::string name = "Default", std::string tag = "None", Application* _app = nullptr);

	GameObject(GameObject& obj);

	~GameObject();

	virtual void OnCollisionEnter(RigidBody* col);
	virtual void OnCollisionExit(RigidBody* col);

	virtual void OnTriggerEnter(RigidBody* col);

	virtual void Start();

	virtual void PreUpdate();

	virtual void Update();

	virtual void PostUpdate();

	virtual void CleanUp();

	bool CompareTag(std::string tag);

	iPoint GetDrawPosition();

	/// <summary>
	/// Si exixte pBody devuelve angluo de pBody, si no el de GameObject 
	/// </summary>
	/// <returns>angulo en degradado</returns>
	float GetDegreeAngle();
	
	/// <summary>
	/// Si exixte pBody devuelve position de pBody, si no el de GameObject (px)
	/// </summary>
	/// <returns>position en pixel</returns>
	iPoint GetWorldPosition();

	iPoint GetScreenPosition();

	/// <summary>
	/// Si exixte pBody devuelve position de pBody, si no {0,0}
	/// </summary>
	fPoint GetLinearVelocity();

	/// <summary>
	/// Si exixte pBody, cambia la position de pBody, si no la de GameObject
	/// </summary>
	/// <param name="pos">= position in pixel</param>
	void SetPosition(iPoint pos);

	/// <summary>
	/// Si exixte pBody, cambia el amgulo de pBody, si no el de GameObject
	/// </summary>
	/// <param name="angle">= angle in deg</param>
	void SetRotation(float angle);

	/// <summary>
	/// Si exixte pBody, cambia la velocidad de pBody, si no saldra un aviso en Console
	/// </summary>
	/// <param name="vel"></param>
	void SetLinearVelocity(fPoint vel);

protected:

	void InitRenderObjectWithXml(std::string texName = "null", int index = 0);

private :
	float rotation = 0;

protected:
	Application* _app = nullptr;

	iPoint position = { 0,0 };

public:

	std::string name;

	std::string tag;

	RigidBody* rBody = nullptr;

	RenderObject renderObjects[MAX_GAMEOBJECT_TEXTURES];

	bool pendingToDelete = false;

	bool adjustToGrid = false;
};

#endif // !GAMEOBJECT_H