#include "SceneGameOver.h"


SceneGameOver::SceneGameOver(Application* app) : Scene(app)
{
}

bool SceneGameOver::Start()
{
    buttonsTex[RESTART_BTN] = _app->textures->Load("Assets/textures/UI/again.png");
    buttonsTex[MENU_BTN] = _app->textures->Load("Assets/textures/UI/menu.png");

    buttonSections[RESTART_BTN] = { 160, 0, 160, 80 };
    buttonSections[MENU_BTN] = { 0, 0, 160, 80 };

    btnState = RESTART_BTN;

    return true;
}

bool SceneGameOver::Update()
{
    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        NextBtnState();
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
    {
        LastBtnState();
    }

    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        switch (btnState)
        {
        case GAMEOVERBUTTONSTATE::RESTART_BTN:
            _app->scene->ChangeCurrentScene(SCENES::SCENE_GAME, 0);
            break;

        case GAMEOVERBUTTONSTATE::MENU_BTN:
            _app->scene->ChangeCurrentScene(SCENES::SCENE_MENU, 0);
            break;
        }
    }

    return true;
}

bool SceneGameOver::PostUpdate()
{
    _app->renderer->AddTextureRenderQueue(buttonsTex[RESTART_BTN], { 280,200 }, buttonSections[RESTART_BTN], 0.5f, 1);
    _app->renderer->AddTextureRenderQueue(buttonsTex[MENU_BTN], { 280,250 }, buttonSections[MENU_BTN], 0.5f, 1);

    return true;
}

void SceneGameOver::NextBtnState()
{
    // Change btn state
    btnState = (GAMEOVERBUTTONSTATE)(btnState + 1);
    if (btnState == GAMEOVERBUTTONSTATE::MAX2) btnState = GAMEOVERBUTTONSTATE::RESTART_BTN;

    // Change btn textures seccions
    for (int i = 0; i < 2; i++) buttonSections[i] = { 0,0,160,80 };
    buttonSections[btnState] = { 160,0,160,80 };
}

void SceneGameOver::LastBtnState()
{
    // Change btn state
    if (btnState == GAMEOVERBUTTONSTATE::RESTART_BTN) btnState = (GAMEOVERBUTTONSTATE)(GAMEOVERBUTTONSTATE::MAX2 - 1);
    else btnState = (GAMEOVERBUTTONSTATE)(btnState - 1);

    // Change btn textures seccions
    for (int i = 0; i < 2; i++) buttonSections[i] = { 0,0,160,80 };
    buttonSections[btnState] = { 160,0,160,80 };
}