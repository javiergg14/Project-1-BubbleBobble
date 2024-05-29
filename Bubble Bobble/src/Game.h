#pragma once
#include "Globals.h"
#include "Scene.h"
#include "FadeTransition.h"


class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state1;
    Scene* scene;
    const Texture2D* img_menu;
    const Texture2D* img_title;
    const Texture2D* img_win;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
    FadeTransition fade_transition;
};