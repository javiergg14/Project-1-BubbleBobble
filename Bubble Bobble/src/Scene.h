#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "EnemyManager.h"
#include "ShotManager.h"
#include "ParticleManager.h"
#include "Text.h"



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
    bool ScoreCheck();
    bool VidaCheck();
    AABB Send();

private:
    AppStatus LoadLevel(int stage);

    void CheckCollisions();
    void ClearLevel();
    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player* player;

    EnemyManager* enemies;

    ShotManager* shots;

    ParticleManager* particles;
    
    TileMap* level;
    std::vector<Object*> objects;

    Camera2D camera;
    DebugMode debug;

    Text* font1;
};

