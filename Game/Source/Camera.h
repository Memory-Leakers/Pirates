#pragma once
class Application;
class GameObject;
#include "Point.h"

class Camera
{
public:
	Camera(Application* app, GameObject* target = nullptr);

	void Start();

	void Update();

	void UpdatePosition();

	void SetTarget(GameObject * target);

	void ReleaseTarget();

	void MoveCameraWithMouse();

private:

	iPoint GetCenter();

	Application* App;

	GameObject* target;

	int pivotX, pivotY;

	int distanceFromTarget;

public:
	int x = 0;
	int y = 0;
	int w;
	int h;

	int cameraSpeed = 1;

	int cameraDelay = 20;

	int moveX = 1, moveY = 1;

	int mapHeight, mapWidth;

};

