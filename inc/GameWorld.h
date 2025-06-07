#pragma once
#include "raylib.h"
#include "GameClock.h"
#include "Tile.h"
#include "Map.h"
#include "ResrcManager.h"
#include "Mario.h"
#include "MediatorCollision.h"

// Forward declarations
class Mario;
class MediatorCollision;

class GameWorld {
    private:
        static constexpr float gravity = 1800;
        Texture2D background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");
        Map map;
        float currBackgroundStarX = 0.0f;

        MediatorCollision mediatorCollision;
        Mario player;
        Camera2D camera;
        std::vector<Tile*> &interactiveTiles;
    public:
        GameWorld();
        ~GameWorld();

        void UpdateWorld();
        void DrawWorld();

        static const float GetGravity();
        static void Init();
};