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

	bool CheckMaximumBoundaries();

private:

	iPoint GetCenter();

	Application* App = nullptr;

	GameObject* target = nullptr;

	int pivotX = 0, pivotY = 0;

	int distanceFromTarget = 0;

public:
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;

	int cameraSpeed = 1;

	int cameraDelay = 20;

	int moveX = 1, moveY = 1;

	int mapHeight = 304 , mapWidth = 1860;

	bool reachedMax = 0;
};

