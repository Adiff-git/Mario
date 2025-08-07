#include "../inc/SaveManager.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ctime>
#include "raylib.h"

// JSON library - bạn cần thêm nlohmann/json
#include "json.hpp"
using json = nlohmann::json;

SaveManager* SaveManager::instance = nullptr;

SaveManager::SaveManager() {
    saveDirectory = "resources/saves/";
    CreateSaveDirectory();
}

SaveManager& SaveManager::GetInstance() {
    if (instance == nullptr) {
        instance = new SaveManager();
    }
    return *instance;
}

void SaveManager::Destroy() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

bool SaveManager::SaveGame(const GameSaveData& data, int slotIndex) {
    if (slotIndex < 1 || slotIndex > 3) {
        std::cout << "Invalid save slot: " << slotIndex << std::endl;
        return false;
    }
    
    try {
        json j;
        
        // Convert save data to JSON
        j["saveDateTime"] = data.saveDateTime;
        j["isMultiplayer"] = data.isMultiplayer;
        j["currentLevel"] = data.currentLevel;
        j["gameTime"] = data.gameTime;
        
        // Player 1 data
        j["player1"]["characterType"] = data.player1.characterType;
        j["player1"]["x"] = data.player1.x;
        j["player1"]["y"] = data.player1.y;
        j["player1"]["velX"] = data.player1.velX;
        j["player1"]["velY"] = data.player1.velY;
        j["player1"]["lives"] = data.player1.lives;
        j["player1"]["coins"] = data.player1.coins;
        j["player1"]["score"] = data.player1.score;
        j["player1"]["playerState"] = data.player1.playerState;
        j["player1"]["additionalState"] = data.player1.additionalState;
        
        // Player 2 data (if multiplayer)
        if (data.isMultiplayer) {
            j["player2"]["characterType"] = data.player2.characterType;
            j["player2"]["x"] = data.player2.x;
            j["player2"]["y"] = data.player2.y;
            j["player2"]["velX"] = data.player2.velX;
            j["player2"]["velY"] = data.player2.velY;
            j["player2"]["lives"] = data.player2.lives;
            j["player2"]["coins"] = data.player2.coins;
            j["player2"]["score"] = data.player2.score;
            j["player2"]["playerState"] = data.player2.playerState;
            j["player2"]["additionalState"] = data.player2.additionalState;
        }
        
        // Enemies
        j["enemies"] = json::array();
        for (const auto& enemy : data.enemies) {
            json enemyJson;
            enemyJson["enemyType"] = enemy.enemyType;
            enemyJson["x"] = enemy.x;
            enemyJson["y"] = enemy.y;
            enemyJson["velX"] = enemy.velX;
            enemyJson["velY"] = enemy.velY;
            enemyJson["state"] = enemy.state;
            j["enemies"].push_back(enemyJson);
        }
        
        // Items
        j["items"] = json::array();
        for (const auto& item : data.items) {
            json itemJson;
            itemJson["itemType"] = item.itemType;
            itemJson["x"] = item.x;
            itemJson["y"] = item.y;
            itemJson["velX"] = item.velX;
            itemJson["velY"] = item.velY;
            itemJson["isCollected"] = item.isCollected;
            j["items"].push_back(itemJson);
        }
        
        // Blocks
        j["blocks"] = json::array();
        for (const auto& block : data.blocks) {
            json blockJson;  // Thêm dòng này - khai báo biến blockJson
            blockJson["blockType"] = block.blockType;
            blockJson["x"] = block.x;
            blockJson["y"] = block.y;
            blockJson["state"] = block.state;
            blockJson["hasBeenHit"] = block.hasBeenHit;
            blockJson["giftType"] = block.giftType;
            j["blocks"].push_back(blockJson);
        }

        j["tiles"] = json::array();
        for (const auto& tile : data.tiles) {
            json tileJson;
            tileJson["tileType"] = tile.tileType;
            tileJson["x"] = tile.x;
            tileJson["y"] = tile.y;
            j["tiles"].push_back(tileJson);
        }
        
        // Write to file
        std::ofstream file(GetSaveFilePath(slotIndex));
        if (!file.is_open()) {
            std::cout << "Failed to open save file for writing" << std::endl;
            return false;
        }
        
        file << j.dump(4);  // Pretty print with 4 spaces
        file.close();
        
        std::cout << "Game saved successfully to slot " << slotIndex << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::LoadGame(GameSaveData& data, int slotIndex) {
    if (slotIndex < 1 || slotIndex > 3) {
        std::cout << "Invalid save slot: " << slotIndex << std::endl;
        return false;
    }
    
    try {
        std::ifstream file(GetSaveFilePath(slotIndex));
        if (!file.is_open()) {
            std::cout << "Save file not found for slot " << slotIndex << std::endl;
            return false;
        }
        
        json j;
        file >> j;
        file.close();
        
        // Load basic data
        data.saveDateTime = j["saveDateTime"];
        data.isMultiplayer = j["isMultiplayer"];
        data.currentLevel = j["currentLevel"];
        data.gameTime = j["gameTime"];
        
        // Load player 1
        data.player1.characterType = j["player1"]["characterType"];
        data.player1.x = j["player1"]["x"];
        data.player1.y = j["player1"]["y"];
        data.player1.velX = j["player1"]["velX"];
        data.player1.velY = j["player1"]["velY"];
        data.player1.lives = j["player1"]["lives"];
        data.player1.coins = j["player1"]["coins"];
        data.player1.score = j["player1"]["score"];
        data.player1.playerState = j["player1"]["playerState"];
        data.player1.additionalState = j["player1"]["additionalState"];
        
        // Load player 2 if multiplayer
        if (data.isMultiplayer && j.contains("player2")) {
            data.player2.characterType = j["player2"]["characterType"];
            data.player2.x = j["player2"]["x"];
            data.player2.y = j["player2"]["y"];
            data.player2.velX = j["player2"]["velX"];
            data.player2.velY = j["player2"]["velY"];
            data.player2.lives = j["player2"]["lives"];
            data.player2.coins = j["player2"]["coins"];
            data.player2.score = j["player2"]["score"];
            data.player2.playerState = j["player2"]["playerState"];
            data.player2.additionalState = j["player2"]["additionalState"];
        }
        
        // Load enemies
        data.enemies.clear();
        for (const auto& enemyJson : j["enemies"]) {
            EnemySave enemy;
            enemy.enemyType = enemyJson["enemyType"];
            enemy.x = enemyJson["x"];
            enemy.y = enemyJson["y"];
            enemy.velX = enemyJson["velX"];
            enemy.velY = enemyJson["velY"];
            enemy.state = enemyJson["state"];
            data.enemies.push_back(enemy);
        }
        
        // Load items
        data.items.clear();
        for (const auto& itemJson : j["items"]) {
            ItemSave item;
            item.itemType = itemJson["itemType"];
            item.x = itemJson["x"];
            item.y = itemJson["y"];
            item.velX = itemJson["velX"];
            item.velY = itemJson["velY"];
            item.isCollected = itemJson["isCollected"];
            data.items.push_back(item);
        }
        
        // Load blocks
        data.blocks.clear();
        for (const auto& blockJson : j["blocks"]) {
            BlockSave block;
            block.blockType = blockJson["blockType"];
            block.x = blockJson["x"];
            block.y = blockJson["y"];
            block.state = blockJson["state"];
            block.hasBeenHit = blockJson["hasBeenHit"];
            block.giftType = blockJson["giftType"];
            data.blocks.push_back(block);
        }

        data.tiles.clear();
        for (const auto& tileJson : j["tiles"]) {
            TileSave tile;
            tile.tileType = tileJson["tileType"];
            tile.x = tileJson["x"];
            tile.y = tileJson["y"];
            data.tiles.push_back(tile);
        }
        
        std::cout << "Game loaded successfully from slot " << slotIndex << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cout << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::HasSaveData(int slotIndex) {
    return std::filesystem::exists(GetSaveFilePath(slotIndex));
}

bool SaveManager::DeleteSave(int slotIndex) {
    try {
        return std::filesystem::remove(GetSaveFilePath(slotIndex));
    } catch (const std::exception& e) {
        std::cout << "Error deleting save: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveManager::GetSaveFileName(int slotIndex) {
    return "save_slot_" + std::to_string(slotIndex) + ".json";
}

std::string SaveManager::GetSaveDisplayName(int slotIndex) {
    return "Save Slot " + std::to_string(slotIndex);
}

std::string SaveManager::GetSaveFilePath(int slotIndex) {
    return saveDirectory + GetSaveFileName(slotIndex);
}

bool SaveManager::CreateSaveDirectory() {
    try {
        if (!std::filesystem::exists(saveDirectory)) {
            std::filesystem::create_directories(saveDirectory);
        }
        return true;
    } catch (const std::exception& e) {
        std::cout << "Error creating save directory: " << e.what() << std::endl;
        return false;
    }
}

