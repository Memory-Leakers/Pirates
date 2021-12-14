#pragma once
#include "SDL/include/SDL.h"
#include "Globals.h"

enum RenderType
{
	RENDER_TEXTURE,
	RENDER_RECT,
	RENDER_LINE,
	RENDER_CIRCLE
};

class RenderObject
{
public:

	RenderObject() {};
	~RenderObject() {};

	bool Draw(SDL_Renderer* renderer) 
	{
		switch (type)
		{
		case RenderType::RENDER_TEXTURE:
			if (section.w == 0 || section.h == 0)
			{
				if (SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, rotation, NULL, flip) != 0)
				{
					printf_s("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
				}
			}
			else
			{
				if (SDL_RenderCopyEx(renderer, texture, &section, &destRect, rotation, NULL, flip) != 0)
				{
					printf_s("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
				}
			}

			return true;
			break;
		case RenderType::RENDER_RECT:
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			if (filled)
			{
				SDL_RenderFillRect(renderer, &destRect);
			}
			else
			{
				SDL_RenderDrawRect(renderer, &destRect);
			}
			return true;
			break;
		case RenderType::RENDER_LINE:

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

			SDL_RenderDrawLine(renderer,start.x, start.y, end.x, end.y);
			
			break;
		}
	}

	void InitAsTexture(SDL_Texture* texture, SDL_Rect destRect, SDL_Rect section, int layer, float orderInLayer,
		SDL_RendererFlip flip, float rotation, float scale, float speedRegardCamera)
	{
		this->texture = texture;
		this->section = section;
		this->flip = flip;
		this->rotation = rotation;
		this->destRect = destRect;
		this->type = RENDER_TEXTURE;
		this->layer = layer;
		this->orderInLayer = orderInLayer;
		this->scale = scale;
		this->speedRegardCamera = speedRegardCamera;
	}

	void InitAsRect(SDL_Rect destRect, SDL_Color color = { 0,0,0,255 }, bool filled = false, int layer = 0, float orderInLayer = 0.0f,
		float speedRegardCamera = 1.0f)
	{
		this->color = color;
		this->filled = filled;
		this->destRect = destRect;
		this->type = RENDER_RECT;
		this->layer = layer;
		this->orderInLayer = orderInLayer;
		this->scale = scale;
		this->speedRegardCamera = speedRegardCamera;
	}

	void InitAsLine(iPoint startPoint, iPoint endPoint, SDL_Color color = { 0,0,0,255 }, int layer = 0, float orderInLayer = 0.0f, float speed = 1.0f)
	{
		this->color = color;
		this->start = startPoint;
		this->end = endPoint;
		this->type = RENDER_LINE;
		this->layer = layer;
		this->orderInLayer = orderInLayer;
		this->speedRegardCamera = speed;
	}

	SDL_Rect destRect;
	RenderType type;
	int layer = 0;
	float orderInLayer = 0.0f;
	float scale = 1.0f;
	float speedRegardCamera = 1.0f;
	bool draw = true;
	std::string name = "null";

	// Rect
	SDL_Color color;
	bool filled = false;

	// Line
	iPoint start;
	iPoint end;

	// Texture
	SDL_Texture* texture = nullptr;
	SDL_Rect section = { 0,0,0,0 };
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	float rotation = 0.0f;
	int textureCenterX;
	int textureCenterY;
};
