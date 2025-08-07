#pragma once
#include "raylib.h"
#include "../inc/Item/Item.h"
#include "../inc/World/GameClock.h"
#include "../inc/World/Tile.h"
#include "../inc/World/Map.h"
#include "ResrcManager.h"
#include "../inc/World/MediatorCollision.h"
#include "../inc/Item/CourseClearToken.h"
#include "../inc/Item/FireFlower.h"
#include "../inc/Item/Coin.h"
#include "../inc/Screen/CharacterSelectScreen.h"
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


        void CollectWorldData(GameSaveData& saveData);
        void ApplyLoadedData(const GameSaveData& saveData);
        GameSaveData CollectSaveData();
        
        friend class GameScreen;
};