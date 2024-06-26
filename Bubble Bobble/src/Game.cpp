#include "Game.h"
#include "Globals.h"
#include "PLayer.h"
#include "ResourceManager.h"
#include <stdio.h>
#include "Scene.h"


Game::Game()
{
    state1 = GameState::MAIN_TITLE;
    scene = nullptr;
    img_menu = nullptr;
    img_title = nullptr;
    img_lose = nullptr;
    img_win = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Bubble Bobble");

    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    fade_transition.Set(GameState::MAIN_TITLE, 60, dst);
    
    InitAudioDevice();
    Sound fxOgg = LoadSound("music/Lvl1.ogg");
    PlaySound(fxOgg);


    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    if (data.LoadTexture(Resource::IMG_MENU, "images/Title.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);
    if (data.LoadTexture(Resource::IMG_TITLE, "images/Pretitle.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_title = data.GetTexture(Resource::IMG_TITLE);
    if (data.LoadTexture(Resource::IMG_WIN, "images/Win.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_win = data.GetTexture(Resource::IMG_WIN);
    if (data.LoadTexture(Resource::IMG_LOSE, "images/Lose.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_lose = data.GetTexture(Resource::IMG_LOSE);

    return AppStatus::OK;
}

AppStatus Game::BeginPlay()
{
    scene = new Scene();
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    if (fade_transition.IsActive())
    {
        GameState prev_frame = state1;
        state1 = fade_transition.Update();

        //Begin play and finish play are delayed due to the fading transition effect
        if (prev_frame == GameState::MAIN_MENU && state1 == GameState::PLAYING1)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }
        else if (prev_frame == GameState::PLAYING1 && state1 == GameState::MAIN_MENU)
        {
            FinishPlay();
        }
    }
    else
    {
        switch (state1)
        {
        case GameState::MAIN_TITLE:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                //"state = GameState::PLAYING;" but not until halfway through the transition
                fade_transition.Set(GameState::MAIN_TITLE, 60, GameState::MAIN_MENU, 60, dst);
            }
            break;
        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                //"state = GameState::PLAYING;" but not until halfway through the transition
                fade_transition.Set(GameState::MAIN_MENU, 60, GameState::PLAYING1, 60, dst);
            }
            break;
        case GameState::LOSE:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                //"state = GameState::PLAYING;" but not until halfway through the transition
                fade_transition.Set(GameState::LOSE, 60, GameState::MAIN_MENU, 60, dst);
            }
            break;

        case GameState::PLAYING1:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                //"state = GameState::MAIN_MENU;" but not until halfway through the transition
                fade_transition.Set(GameState::PLAYING1, 60, GameState::MAIN_MENU, 60, dst);
            }
            else if (scene->ScoreCheck() == true)
            {
                fade_transition.Set(GameState::PLAYING1, 60, GameState::WINNING, 60, dst);
            }
            else if (scene->VidaCheck() == true)
            {
                fade_transition.Set(GameState::PLAYING1, 60, GameState::LOSE, 60, dst);
            }
            else
            {
                //Game logic
                scene->Update();
            }
            break;
        case GameState::WINNING:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                //"state = GameState::MAIN_MENU;" but not until halfway through the transition
                fade_transition.Set(GameState::WINNING, 60, GameState::MAIN_MENU, 60, dst);
            }
            break;
        }
    }

    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state1)
    {
    case GameState::MAIN_TITLE:
        DrawTexture(*img_title, 0, 0, WHITE);
        break;
    case GameState::WINNING:
        DrawTexture(*img_win, 0, 0, WHITE);
        break;
    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;
    case GameState::LOSE:
        DrawTexture(*img_lose, 0, 0, WHITE);
        break;

    case GameState::PLAYING1:
        scene->Render();
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    if (fade_transition.IsActive()) fade_transition.Render();
    EndDrawing();
}
void Game::Cleanup()
{
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_TITLE);
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_WIN);
    data.ReleaseTexture(Resource::IMG_LOSE);

    UnloadRenderTexture(target);
}