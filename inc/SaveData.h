#pragma once
#include <vector>
#include <string>
#include <memory>
#include "raylib.h"
#include "../inc/Character/CharacterType.h"


// Save data structures
struct PlayerSave {
    int characterType;  // CharacterType as int
    float x, y;
    float velX, velY;
    int lives;
    int coins;
    int score;
    int playerState;
    int additionalState;
};

struct EnemySave {
    int enemyType;  // EnemyType as int
    float x, y;
    float velX, velY;
    int state;
};

struct ItemSave {
    int itemType;  // ItemType as int
    float x, y;
    float velX, velY;
    bool isCollected;
};

struct BlockSave {
    int blockType;  // BlockType as int
    int giftType;
    float x, y;
    int state;
    bool hasBeenHit;
};

struct GameSaveData {
    std::string saveDateTime;
    PlayerSave player1;
    PlayerSave player2;  // For multiplayer
    bool isMultiplayer;
    std::vector<EnemySave> enemies;
    std::vector<ItemSave> items;
    std::vector<BlockSave> blocks;
    int currentLevel;
    float gameTime;
};