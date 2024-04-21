#pragma once
#include <raylib.h>
#include "Player.h"
#include "Balas.h"
#include "TileMap.h"
#include "Object.h"



enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void CheckCollisionsVida();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player* player;
    
    TileMap* level;
    std::vector<Object*> objects;
    std::vector<Balas*> balas;
    std::vector<Object*> enemies;

    Camera2D camera;
    DebugMode debug;
};

