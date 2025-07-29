#pragma once
#include "Command.h"
#include "CharacterCommands.h"
#include <memory>
#include <unordered_map>

enum class ControlType {
    ARROWS = 1,    // Mũi tên + Space/Z
    WASD = 2       // WASD + Enter/X
};

class InputHandler {
private:
    std::unordered_map<int, std::unique_ptr<Command>> keyBindings;
    std::unique_ptr<Command> nullCommand;
    Character* character;
    ControlType controlType;
    
    void setupArrowControls();
    void setupWASDControls();
    
public:
    InputHandler(Character* character, ControlType type);
    ~InputHandler() = default;
    
    void handleInput();
    void setControlType(ControlType type);
    void bindKey(int key, std::unique_ptr<Command> command);
    //get control type
    ControlType getControlType() const {
        return controlType;
    }
};