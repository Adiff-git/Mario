#pragma once
#include "raylib.h"
#include "Mario.h"
#include "Item.h"
#include "GameClock.h"
#include "Tile.h"
#include "Map.h"
#include "ResrcManager.h"
#include "MediatorCollision.h"
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Coin.h"
#include "GameScreen.h"
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
        Mario player;
        Camera2D camera;
        
        GameState gameState;
        GameScreen* gameScreen;

        std::vector<Tile*> &interactiveTiles;
        std::vector<std::shared_ptr<Item>> interactiveItems;  
    public:
        GameWorld();
        GameWorld(int MapID, GameScreen* gameScreen); 
        ~GameWorld();

        void UpdateWorld();
        void DrawWorld();
        bool IsCompleted();
        GameState GetGameState() ;

        Map GetMap();

        static const float GetGravity();
        static void Init();

        std::vector<std::shared_ptr<Item>>& GetInteractiveItems() {
            return interactiveItems;
        };
        
        friend class GameScreen;
};
