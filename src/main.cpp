#include "raylib.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "Mario.h"
#include "GameClock.h"
#include <iostream>
#include "ScreenController.h"

int main() {
    InitWindow(1600, 900, "Mario Game");
    InitAudioDevice();

    SetTargetFPS(144);
    bool isPaused = false;
    ResrcManager::GetInstance().loadResources();
    ScreenController screenController; // Create a screen controller instance
    // // Create a Mario instance
    // Mario mario(Vector2{100, 100}, 3, SMALL);
    Texture2D *backgroundTexture = &ResrcManager::GetInstance().getTexture("MENU_BACKGROUND");
    GameWorld::Init(); // Initialize game world resources   
    GameWorld gameWorld; // Create a game world instance
    while(!WindowShouldClose()) {
        GameClock::GetInstance().updateTimeAcum += GetFrameTime();

        
         // Update game world
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // gameWorld.UpdateWorld();
        // gameWorld.DrawWorld();
        screenController.Update();
        screenController.Draw();
        
        EndDrawing();
    }
}