#include "raylib.h"
#include "ResrcManager.h"
#include "GameWorld.h"
#include "Mario.h"
#include "GameClock.h"
#include <iostream>

int main() {
    InitWindow(1600, 900, "Mario Game");
    InitAudioDevice();

    SetTargetFPS(144);
    bool isPaused = false;
    ResrcManager::GetInstance().loadResources();

    // Create a Mario instance
    Mario mario(Vector2{100, 100}, 3, SMALL);

    while(!WindowShouldClose()) {
        // if (IsKeyPressed(KEY_P)) {
        //     isPaused = !isPaused;
        // }

        // if (!isPaused) {
            GameClock::getInstance().updateTimeAcum += GetFrameTime();

        // }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        mario.draw(); // Draw Mario
        // mario.UpdateStateAndPhysic(); // Update Mario's state and physics
        EndDrawing();
    }
}