#pragma once
#include "Module.h"
#include "Globals.h"


#define MAX_MOUSE_BUTTONS 5
#define MAX_KEYS 300

enum KEY_STATE
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP,
	KEY_ERROR
};

class ModuleInput : public Module
{
public:
	
	ModuleInput(Application* app, bool start_enabled = true);
	~ModuleInput();

	bool Init(pugi::xml_node&);
	UpdateStatus PreUpdate() override;
	bool CleanUp();

	KEY_STATE GetKey(uint id) const
	{
		return (id < MAX_KEYS) ? keyboard[id] : KEY_ERROR;
	}

	KEY_STATE GetMouseButton(uint id) const
	{
		return (id < MAX_MOUSE_BUTTONS) ? mouse_buttons[id] : KEY_ERROR;
	}

	int GetMouseX();

	int GetMouseY();

private:
	KEY_STATE* keyboard = nullptr;
	KEY_STATE mouse_buttons[MAX_MOUSE_BUTTONS];
	int mouse_x;
	int mouse_y;
};