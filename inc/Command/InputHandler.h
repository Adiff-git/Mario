#pragma once
#include <memory>
#include <unordered_map>
#include <stdexcept>

// Forward declarations
class Command;
class Character;

enum class ControlType {
    ARROWS = 1,    
    WASD = 2       
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
    ~InputHandler() ;
    
    // Delete copy constructor và copy assignment
    InputHandler(const InputHandler&) = delete;
    InputHandler& operator=(const InputHandler&) = delete;
    
    // Delete move constructor và move assignment để tránh lỗi
    InputHandler(InputHandler&&) = delete;
    InputHandler& operator=(InputHandler&&) = delete;

    void handleInput();
    void setControlType(ControlType type);
    void bindKey(int key, std::unique_ptr<Command> command);
    
    ControlType getControlType() const {
        return controlType;
    }
};