#include "SceneGameOver.h"


SceneGameOver::SceneGameOver(Application* app) : Scene(app)
{
}

bool SceneGameOver::Start()
{
    //mainMenu = _app->textures->Load("Assets/textures/Menu/GameOverOptions.png", false);
    //arrow = _app->textures->Load("Assets/textures/Menu/ArrowGO.png", false);

    arrowPos = { 20, 190 };

    _app->SaveGameRequest();

    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Start();
    }

    return true;
}

bool SceneGameOver::Update()
{
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->Update();
    }

    if (_app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
    {
        arrowPos.y = arrowPos.y == 250 ? 190 : 250;
    }
    if (_app->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
    {
        arrowPos.y = arrowPos.y == 190 ? 250 : 190;
    }

    if (_app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || _app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
    {
        if (arrowPos.y == 190)
        {
            _app->scene->ChangeCurrentScene(0, 0);
        }
        else
        {
            _app->scene->ChangeCurrentScene(1, 0);
        }
        //SelectDifficulty();
    }

    return true;
}

bool SceneGameOver::PostUpdate()
{
    for (int i = 0; i < gameObjects.count(); i++)
    {
        if (gameObjects[i] != nullptr)
            gameObjects[i]->PostUpdate();
    }
    //_app->renderer->AddTextureRenderQueue(mainMenu, { 0,0, });
    //_app->renderer->AddTextureRenderQueue(arrow, arrowPos, { 0,0,0,0 }, 1, 1);

    return true;
}

void SceneGameOver::SetSaveData()
{
    playerX = playerStartPos.x;
    playerY = playerStartPos.y;

    _app->scene->ResetPlayerSettings();
}
