#include "../inc/World/GameClock.h"
GameClock *GameClock::instance = nullptr;
GameClock &GameClock::GetInstance()
{
    if(instance == nullptr) {
        instance = new GameClock();
        instance->updateTimeAcum = 0.0;
    }
    return *instance;
}

// Add destructor to prevent memory leaks
GameClock::~GameClock() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}




