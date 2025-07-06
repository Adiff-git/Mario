#pragma once

enum StateType {
    STATE_MENU,
    STATE_MAP1,
    STATE_MAP2,
    STATE_PAUSE,
    STATE_GAME_OVER
};

class GameState {
    public:
        virtual ~GameState() = default;
        virtual void Enter() = 0;
        virtual void Update() = 0;
        virtual void Draw() = 0;
        virtual void Exit() = 0;
        virtual void HandleInput() = 0;
};
    