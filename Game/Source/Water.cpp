#include "Water.h"

Water::Water(iPoint pos, std::string name, std::string tag, Application* app) : GameObject(name, tag, app)
{
	renderObjects[0].texture = _app->textures->Load("Assets/textures/Terrain/Water.png");
	renderObjects[0].destRect.x = pos.x - 128;
	renderObjects[0].destRect.y = pos.y;

	this->rBody = new RigidBody({ pos.x, pos.y + 150 }, WATER, 3200, 300);
}

void Water::Update()
{
	if (renderObjects[0].destRect.x >= 0)
	{
		renderObjects[0].destRect.x -= 128;
	}
	else 
	{
		renderObjects[0].destRect.x++;
	}
}

void Water::PostUpdate()
{
	_app->renderer->AddTextureRenderQueue(renderObjects[0].texture, { renderObjects[0].destRect.x, renderObjects[0].destRect.y }, { 0,0,0,0 }, 1, 2);
}
