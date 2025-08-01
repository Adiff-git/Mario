#pragma once
#include "raylib.h"
#include "Item.h"
#include "GameClock.h"
#include "Tile.h"
#include "Map.h"
#include "ResrcManager.h"
#include "MediatorCollision.h"
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Coin.h"
#include "CharacterSelectScreen.h"
class Character;
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
        

        Camera2D camera;
        
        GameState gameState;
        GameScreen* gameScreen;

        bool isMultiplayer;
        CharacterType player1Character;
        CharacterType player2Character;

        std::vector<Tile*> &interactiveTiles;
    public:

        Character* player1;
        Character* player2;

        GameWorld();
        GameWorld(int level, GameScreen* gameScreen);
        GameWorld(int MapID, GameScreen* gameScreen, bool multiplayer, CharacterType p1Type, CharacterType p2Type); 
        ~GameWorld();

        void UpdateWorld();
        void DrawWorld();
        bool IsCompleted();
        GameState GetGameState() ;

        Map* GetMap();

        static const float GetGravity();
        static void Init();
        friend class GameScreen;

        Character* GetPlayer1();
        Character* GetPlayer2();
        bool IsMultiplayer() ;

        friend class GameScreen;
};