#include "raylib.h"
#include "ResrcManager.h"
#include "../inc/World/GameWorld.h"
#include "../inc/Character/Character.h"
#include "../inc/World/GameClock.h"
#include <iostream>
#include "../inc/Screen/ScreenController.h"
#include"SoundManager.h"

int main() {
    InitWindow(1600, 900, "Mario Game");
    InitAudioDevice();

    SetTargetFPS(60);
    bool isPaused = false;
    ResrcManager::GetInstance().loadResources();
    ScreenController screenController; // Create a screen controller instance
    // // Create a Mario instance
    Texture2D *backgroundTexture = &ResrcManager::GetInstance().getTexture("MENU_BACKGROUND");
    // GameWorld::Init(); // Initialize game world resources   
    // GameWorld gameWorld; // Create a game world instance
    while(!WindowShouldClose()) {
        GameClock::GetInstance().updateTimeAcum += 1/60;
        // Update game logic TRƯỚC khi vẽ
        screenController.Update();
        SoundManager::GetInstance().Update(); // Chuyển lên đây
        
        // Drawing
        BeginDrawing();
        ClearBackground(BLUE);
        screenController.Draw();
        EndDrawing();
    }
    ResrcManager::GetInstance().unloadResources();
    CloseAudioDevice();
    CloseWindow(); // Close the window and exit the application
}