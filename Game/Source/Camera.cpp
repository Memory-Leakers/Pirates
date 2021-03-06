#include "Camera.h"
#include "Application.h"
#include "GameObject.h"

Camera::Camera(Application* app, GameObject* target)
{
	App = app;
	this->target = target;
}

void Camera::Start()
{
	pivotX = App->window->width / 2;
	pivotY = App->window->height / 2;
}

void Camera::Update()
{
	UpdatePosition();

	/*if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN)
	{
		y -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
	{
		y += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		x -= cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n",x, y);
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		x += cameraSpeed;
		printf_s("Camera_X: %d, Camera_Y: %d\n", x, y);
	}*/
}

void Camera::UpdatePosition()
{
	if (target != nullptr && !App->scene->isChangingScene)
	{
		// Update Y
		int targetPosY = target->GetWorldPosition().y * App->window->scale;
		targetPosY = (targetPosY - pivotY) * moveY;

		int distance = abs(targetPosY - y);

		targetPosY > y ? y += distance / cameraDelay : targetPosY < y ? y -= distance / cameraDelay : y = y;

		if ((y < 0 || y > mapHeight) && distance > 5) reachedMax = true;
		else reachedMax = false;

		y < 0 ? y = 0 : y > mapHeight ? y = mapHeight : y = y;

		// Update X

		int targetPosX = target->GetWorldPosition().x * App->window->scale;
		targetPosX = (targetPosX - pivotX) * moveX;
	
		distance = abs(targetPosX - x);

		targetPosX > x ? x += distance / cameraDelay : targetPosX < x ? x -= distance / cameraDelay : x = x;

		if ((x < 0 || y > mapWidth) && distance > 5) reachedMax = true;
		else reachedMax = false;

		x < 0 ? x = 0 : x > mapWidth ? x = mapWidth : x = x;
	}
	
}

void Camera::SetTarget(GameObject* target)
{
	this->target = target;
}

void Camera::ReleaseTarget()
{
	target = nullptr;
	x = y = 0;
}

void Camera::MoveCameraWithMouse()
{
	if (target != nullptr) return;
	
	int leftBorder = App->window->width / 8; //	160
	int rightBorder = App->window->width - leftBorder; // 1280 - 160
	int upBorder = App->window->height / 8;
	int downBorder = App->window->height - upBorder;

	if (App->input->GetMouseX() < leftBorder)
	{
		//Move camera left
		x -= 10;
	}
	if (App->input->GetMouseX() > rightBorder)
	{
		x += 10;
	}
	if (App->input->GetMouseY() < upBorder)
	{
		//Move camera left
		y -= 10;
	}
	if (App->input->GetMouseY() > downBorder)
	{
		y += 10;
	}
	y < 0 ? y = 0 : y > mapHeight ? y = mapHeight : y = y;
	x < 0 ? x = 0 : x > mapWidth ? x = mapWidth : x = x;

}

bool Camera::CheckMaximumBoundaries()
{
	if (x == 0 || y == 0)
		return true;

	if (x == mapWidth || y == mapHeight)
		return true;

	return false;
}

iPoint Camera::GetCenter()
{
	return {x + pivotX, y + pivotY };
}


