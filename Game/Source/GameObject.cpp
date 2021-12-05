#include "GameObject.h"

GameObject::GameObject()
{
}

//TODO: Poner Applicationi* en constructor
GameObject::GameObject(std::string name, std::string tag , Application* _app)
{
	this->name = name;
	this->_app = _app;
	this->tag = tag;
}

GameObject::GameObject(GameObject& obj)
{
	this->name = obj.name;
	this->tag = obj.tag;
}

GameObject::~GameObject()
{

}

void GameObject::OnCollisionEnter(RigidBody* col)
{
}

void GameObject::OnCollisionExit(RigidBody* col)
{

}

void GameObject::OnTriggerEnter(RigidBody* col)
{
}

void GameObject::Start()
{
	//Cargar texturas
}

void GameObject::PreUpdate()
{
}

void GameObject::Update()
{
}

void GameObject::PostUpdate()
{
	// Dibujar texturas
	for (int i = 0; i < MAX_GAMEOBJECT_TEXTURES; i++)
	{
		if (renderObjects[i].texture != nullptr && renderObjects[i].draw)
		{
			renderObjects[i].destRect.x = GetDrawPosition().x;
			renderObjects[i].destRect.y = GetDrawPosition().y;
			renderObjects[i].rotation = GetDegreeAngle();
			
			_app->renderer->AddTextureRenderQueue(renderObjects[i].texture, { renderObjects[i].destRect.x,renderObjects[i].destRect.y },
			renderObjects[i].section, renderObjects[i].scale, renderObjects[i].layer, renderObjects[i].orderInLayer,
			renderObjects[i].rotation, renderObjects[i].flip, renderObjects[i].speedRegardCamera);
		}
	}
}

void GameObject::CleanUp()
{
}

bool GameObject::CompareTag(std::string tag)
{
	if(this->tag == tag)
	{
		return true;
	}

	return false;
}

iPoint GameObject::GetDrawPosition()
{

	return this->position;
}

float GameObject::GetDegreeAngle()
{
	
	return this->rotation;
}

iPoint GameObject::GetWorldPosition()
{
	iPoint pos = { (int)rBody->GetPosition().x, (int)rBody->GetPosition().y };
	return pos;
}

iPoint GameObject::GetScreenPosition()
{
	iPoint pos = { (int)rBody->GetPosition().x, (int)rBody->GetPosition().y };
	pos.x = (int)(-_app->renderer->camera->x + pos.x) * _app->window->scale;
	pos.y = (int)(-_app->renderer->camera->y + pos.y) * _app->window->scale;
	return pos;
}


fPoint GameObject::GetLinearVelocity()
{
	
	return { 0,0 };
}

void GameObject::SetPosition(iPoint pos)
{

}

void GameObject::SetRotation(float angle)
{

}

void GameObject::SetLinearVelocity(fPoint vel)
{
	
}

void GameObject::InitRenderObjectWithXml(std::string texName, int index)
{
	if (texName == "null") texName = name;

	renderObjects[index].name = texName;
	renderObjects[index].texture = _app->textures->Load(texName, true);
	renderObjects[index].destRect.w = _app->textures->config.child(texName.c_str()).attribute("width").as_int();
	renderObjects[index].destRect.h = _app->textures->config.child(texName.c_str()).attribute("height").as_int();
	renderObjects[index].layer = _app->textures->config.child(texName.c_str()).attribute("layer").as_int(0);
	renderObjects[index].orderInLayer = _app->textures->config.child(texName.c_str()).attribute("orderInLayer").as_float(1.0);
	renderObjects[index].scale = _app->textures->config.child(texName.c_str()).attribute("scale").as_float(1);
	renderObjects[index].section.x = _app->textures->config.child(texName.c_str()).attribute("sectionX").as_int(0);
	renderObjects[index].section.y = _app->textures->config.child(texName.c_str()).attribute("sectionY").as_int(0);
	renderObjects[index].textureCenterX = (renderObjects[index].destRect.w / 2) * renderObjects[index].scale;
	renderObjects[index].textureCenterY = (renderObjects[index].destRect.h / 2) * renderObjects[index].scale;
}
