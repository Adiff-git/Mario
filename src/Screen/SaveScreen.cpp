#include "../inc/Screen/SaveScreen.h"
#include "../inc/Screen/MenuScreen.h"
#include "../inc/Screen/GameScreen.h"
#include "ResrcManager.h"
#include <algorithm>
#include <iostream>

static Texture2D* GetButtonTexture(const std::string& key){
    return &ResrcManager::GetInstance().getTexture(key);
}

SaveScreen::SaveScreen(ScreenController* controller)
    : Screen(controller), backButton(Vector2{50,50}, Vector2{120,50}, "BACK", 20, 1)
{
    backButton.SetTexture(ResrcManager::GetInstance().getTexture("BACK_BUTTON"));
    ScanSaves();
    // Create buttons (max 8 per page vertically)
    int startY = 150;
    int idx = 0;
    for(const auto& entry : saves){
        Button btn(Vector2{ 200.0f, (float)(startY + idx * 70) }, Vector2{600, 60}, entry.name, 24, 1);
        btn.SetTexture(ResrcManager::GetInstance().getTexture("MEM"));
        saveButtons.push_back(std::move(btn));
        idx++;
        if(idx>=8) break; // simple first page
    }
}

void SaveScreen::ScanSaves(){
    saves.clear();
    std::filesystem::path dir = "resources/save";
    if(!std::filesystem::exists(dir)){
        std::cout << "[SaveScreen] No save directory yet (resources/save)." << std::endl;
        return;
    }
    for(auto& p : std::filesystem::directory_iterator(dir)){
        if(p.is_regular_file() && p.path().extension() == ".json"){
            SaveEntry e;
            e.path = p.path().string();
            e.name = p.path().stem().string();
            std::error_code ec; e.time = std::filesystem::last_write_time(p, ec);
            saves.push_back(std::move(e));
        }
    }
    std::sort(saves.begin(), saves.end(), [](const SaveEntry& a, const SaveEntry& b){return a.time > b.time;});
}

void SaveScreen::Update(){
    backButton.Update();
    if(backButton.IsPressed()){
        screenController->ChangeScreen(new MenuScreen(screenController));
        return;
    }
    for(size_t i=0;i<saveButtons.size();++i){
        saveButtons[i].Update();
        if(saveButtons[i].IsPressed()){
            selectedIndex = (int)i;
            // Launch GameScreen with selected save path
            if(selectedIndex >=0 && selectedIndex < (int)saves.size()){
                screenController->ChangeScreen(new GameScreen(screenController, saves[selectedIndex].path));
                return;
            }
        }
    }
}

void SaveScreen::Draw(){
    ClearBackground(RAYWHITE);
    DrawText("Select a Saved Map", 200, 80, 40, BLACK);
    backButton.Draw();
    for(auto& b : saveButtons){
        b.Draw();
    }
    if(saveButtons.empty()){
        DrawText("(No saves found)", 200, 160, 24, DARKGRAY);
    }
}
