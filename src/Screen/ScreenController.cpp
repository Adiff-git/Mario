#include "../inc/Screen/ScreenController.h"
#include "../inc/Screen/MenuScreen.h"
#include "SoundManager.h"
ScreenController::ScreenController() : currentScreen(new MenuScreen(this)) {
    
}
ScreenController::~ScreenController() {
    delete currentScreen; // Clean up the current screen
}

void ScreenController::Update() {
    if (currentScreen) {
        currentScreen->Update();
    }
} 
void ScreenController::Draw() {
    if (currentScreen) {
        currentScreen->Draw();
    }
}

void ScreenController::ChangeScreen(Screen* newScreen) {
    if (currentScreen) {
        delete currentScreen; // Clean up the old screen
    }
    currentScreen = newScreen; // Set the new screen
    if (currentScreen) {
        currentScreen->screenController = this; // Set the screen controller for the new screen
    }
}

