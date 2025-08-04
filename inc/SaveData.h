// #pragma once

// #include <vector>
// #include <string>
// #include "raylib.h"
// #include "Object.h"

// class PlayerSave {
//     public: 
//     float x, y;
//     int lives,
//         coins,
//         score;
//     ObjectState playerState;
//     ObjectState AdditionalState;
//     int type;
// };

// class EnemySave {
//     public: 
//     std::string type;
//     float x, y;
//     bool  alive;
//     ObjectState state;
// };

// class BlockSave {
//     public: 
//     std::string type;
//     float x, y;
//     bool isActive;
//     ObjectState state;
// };

// class ItemSave {
//     public: 
//     std::string type;
//     float x, y;
//     bool isActive;
//     ObjectState state;
// };

// class SaveData {
// public: 
//     int level;
//     float time;
//     std::vector<EnemySave> enemies;
//     std::vector<BlockSave> blocks;
//     std::vector<ItemSave> items;
//     PlayerSave player1;
//     PlayerSave player2; // Chỉ khi multiplayer
//     bool isMultiplayer;
// };