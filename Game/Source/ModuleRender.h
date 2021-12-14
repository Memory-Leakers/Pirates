#pragma once
#include "Module.h"
#include "Point.h"
#include <vector>
#include "RenderObject.h"
#include "Camera.h"

using namespace std;

//struct RenderObject
//{
//	SDL_Texture* texture = nullptr;
//	SDL_Rect section = { 0,0,0,0 };
//	SDL_Rect renderRect;
//	SDL_RendererFlip flip = SDL_FLIP_NONE;
//	float rotation = 0.0f;
//	int layer = 0;
//	float orderInLayer = 0.0f;
//	float speed = 1.0f;
//	float scale = 1.0f;
//};
//
//struct RenderRect
//{
//	SDL_Rect rect;
//	SDL_Color color;
//};

class ModuleRender : public Module
{
public:
	SDL_Renderer* renderer;
	Camera* camera = nullptr;

public:
	ModuleRender(Application* app, bool start_enabled = true);
	~ModuleRender();

	bool Init(pugi::xml_node&);
	UpdateStatus PreUpdate() override;
	UpdateStatus Update();
	UpdateStatus PostUpdate();
	bool CleanUp();

	void AddTextureRenderQueue(SDL_Texture* texture, iPoint pos, SDL_Rect section = {0,0,0,0}, float scale = 1, int layer = 0, float orderInlayer = 0.0f, float rotation = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, float speed = 1.0f);// Speed = 1.0f = Fullscreen camera
	void AddRectRenderQueue(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, int layer = 1, float orderInlayer = 0.0f, bool filled = true, float speed = 1.0f);
	void AddLineRenderQueue(iPoint startPoint, iPoint endPoint, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, int layer = 1, float orderInlayer = 0.0f, float speed = 1.0f);

	int RoundToInt(int num);

#pragma region OBSOLETE
	/// <summary>
	/// DO NOT USE!!!!
	/// </summary>
	/// <param name="obj"></param>
	void AddTextureRenderQueue(RenderObject obj);
	bool Blit(SDL_Texture* texture, int x, int y, float scale = 1, SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true);
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool adjust = false, bool use_camera = false);
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true);
	void CameraMove(iPoint pos);
#pragma endregion

private:
	vector<vector<RenderObject>> renderLayers;
	//vector<RenderRect> rects;

	int gamePixels = 16; // The pixels per texutre used on this game

private:
	void SortRenderObjects(vector<RenderObject>& obj);
	void GetSaveData(pugi::xml_document& save) override;
};