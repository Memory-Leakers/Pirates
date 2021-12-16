#include "SceneMainMenu.h"

SceneMainMenu::SceneMainMenu(Application* app) : Scene(app)
{

}


bool SceneMainMenu::Start()
{
    titleTex = _app->textures->Load("Assets/textures/UI/title.png");

    buttonsTex[PLAY_BTN] = _app->textures->Load("Assets/textures/UI/play.png");
    buttonsTex[EXIT_BTN] = _app->textures->Load("Assets/textures/UI/exit.png");

    buttonSections[PLAY_BTN] = { 160, 0, 160, 80 };
    buttonSections[EXIT_BTN] = { 0, 0, 160, 80 };

    btnState = PLAY_BTN;

    //for (int i = 0; i < gameObjects.count(); i++)
    //{
    //    if (gameObjects[i] != nullptr)
    //        gameObjects[i]->Start();
    //}

    return true;
}

bool SceneMainMenu::Update()
{
    //for (int i = 0; i < gameObjects.count(); i++)
    //{
    //    if (gameObjects[i] != nullptr)
    //    gameObjects[i]->Update();
    //}

   // logoY = logoY == 58 ? logoY : logoY + 1;

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
        case BUTTONSTATE::PLAY_BTN:
            _app->scene->ChangeCurrentScene(1, 0);
            break;

        case BUTTONSTATE::EXIT_BTN:
            return false;
            break;
        }
    }

    return true;
}

bool SceneMainMenu::PostUpdate()
{
    //for (int i = 0; i < gameObjects.count(); i++)
    //{
    //    if (gameObjects[i] != nullptr)
    //        gameObjects[i]->PostUpdate();
    //}

    _app->renderer->AddTextureRenderQueue(buttonsTex[PLAY_BTN], { 280,200 }, buttonSections[PLAY_BTN], 0.5f, 1);
    _app->renderer->AddTextureRenderQueue(buttonsTex[EXIT_BTN], { 280,250 }, buttonSections[EXIT_BTN], 0.5f, 1);
    _app->renderer->AddTextureRenderQueue(titleTex, { 160, 20}, { 0,0,0,0 }, 1, 1);

    return true;
}

void SceneMainMenu::NextBtnState()
{
    // Change btn state
    btnState = (BUTTONSTATE)(btnState + 1);
    if (btnState == BUTTONSTATE::MAX) btnState = BUTTONSTATE::PLAY_BTN;

    // Change btn textures seccions
    for (int i = 0; i < 2; i++) buttonSections[i] = { 0,0,160,80 };
    buttonSections[btnState] = { 160,0,160,80 };
}

void SceneMainMenu::LastBtnState()
{
    // Change btn state
    if (btnState == BUTTONSTATE::PLAY_BTN) btnState = (BUTTONSTATE)(BUTTONSTATE::MAX - 1);
    else btnState = (BUTTONSTATE)(btnState - 1);

    // Change btn textures seccions
    for (int i = 0; i < 2; i++) buttonSections[i] = { 0,0,160,80 };
    buttonSections[btnState] = { 160,0,160,80 };
}
