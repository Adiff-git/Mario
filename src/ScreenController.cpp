#include "ScreenController.h"
#include "MenuScreen.h"

ScreenController::ScreenController() {
    screenStack.push(new MenuScreen(this));
}

ScreenController::~ScreenController() {
    while (!screenStack.empty()) {
        delete screenStack.top();
        screenStack.pop();
    }
}

void ScreenController::Update() {
    if (!screenStack.empty()) {
        screenStack.top()->Update();
    }
}

void ScreenController::Draw() {
    if (!screenStack.empty()) {
        screenStack.top()->Draw();
    }
}

void ScreenController::ChangeScreen(Screen* newScreen) {
    while (!screenStack.empty()) {
        delete screenStack.top();
        screenStack.pop();
    }
    screenStack.push(newScreen);
    if (newScreen) {
        newScreen->screenController = this;
    }
}

void ScreenController::PushScreen(Screen* newScreen) {
    screenStack.push(newScreen);
    if (newScreen) {
        newScreen->screenController = this;
    }
}

void ScreenController::PopScreen() {
    if (!screenStack.empty()) {
        delete screenStack.top();
        screenStack.pop();
    }
}