#pragma once
#include "raylib.h"
#include "Character.h"
#include "Item.h"
#include "GameClock.h"
#include "Tile.h"
#include "Map.h"
#include "ResrcManager.h"
#include "MediatorCollision.h"
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Coin.h"
class GameScreen;
enum class GameState {
    GAME_PLAYING,
    GAME_COMPLETED,
    GAME_OVER,
    GAME_RESET
};

class GameWorld {
    private:
        static constexpr float gravity = 200;
        Texture2D background;
        Map map;
        float BGpos = 0.0f;// 

        MediatorCollision mediatorCollision;
        Character* player;
        Camera2D camera;
        
        GameState gameState;
        GameScreen* gameScreen;

        std::vector<Tile*> &interactiveTiles;
    public:
        GameWorld();
        GameWorld(int MapID, GameScreen* gameScreen); 
        ~GameWorld();

        void UpdateWorld();
        void DrawWorld();
        bool IsCompleted();
        GameState GetGameState() ;

        Map* GetMap();

        static const float GetGravity();
        static void Init();
        friend class GameScreen;
};
