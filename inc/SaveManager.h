// #pragma once
// #include "SaveData.h"
// #include <string>

// class SaveManager {
// private:
//     static SaveManager* instance;
//     std::string saveDirectory;
    
//     SaveManager();

// public:
//     static SaveManager& GetInstance();
//     static void Destroy();
    
//     // Save/Load operations
//     bool SaveGame(const GameSaveData& data, int slotIndex);
//     bool LoadGame(GameSaveData& data, int slotIndex);
//     bool HasSaveData(int slotIndex);
//     bool DeleteSave(int slotIndex);
    
//     // Utility
//     std::string GetSaveFileName(int slotIndex);
//     std::string GetSaveDisplayName(int slotIndex);
    
// private:
//     std::string GetSaveFilePath(int slotIndex);
//     bool CreateSaveDirectory();
// };