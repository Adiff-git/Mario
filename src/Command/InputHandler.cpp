#include "../inc/Command/InputHandler.h"
#include "raylib.h"
#include "../inc/Character/Character.h"
#include "../inc/Command/CharacterCommands.h"

InputHandler::InputHandler(Character* character, ControlType type) 
    : character(character), controlType(type) {
    if (!character) {
        throw std::invalid_argument("Character cannot be null");
    }
    
    nullCommand = std::make_unique<NullCommand>();
    
    if (type == ControlType::ARROWS) {
        setupArrowControls();
    } else {
        setupWASDControls();
    }
}

// QUAN TRỌNG: Định nghĩa destructor trong .cpp file
InputHandler::~InputHandler() {
    // Destructor sẽ tự động dọn dẹp unique_ptr
}

void InputHandler::setupArrowControls() {
    keyBindings.clear();
    if (character) {
        keyBindings[KEY_LEFT] = std::make_unique<MoveLeftCommand>(character);
        keyBindings[KEY_RIGHT] = std::make_unique<MoveRightCommand>(character);
        keyBindings[KEY_UP] = std::make_unique<JumpCommand>(character);
        keyBindings[KEY_DOWN] = std::make_unique<DuckCommand>(character);
        keyBindings[KEY_SPACE] = std::make_unique<FireCommand>(character);
    }
}

void InputHandler::setupWASDControls() {
    keyBindings.clear();
    if (character) {
        keyBindings[KEY_A] = std::make_unique<MoveLeftCommand>(character);
        keyBindings[KEY_D] = std::make_unique<MoveRightCommand>(character);
        keyBindings[KEY_W] = std::make_unique<JumpCommand>(character);
        keyBindings[KEY_S] = std::make_unique<DuckCommand>(character);
        keyBindings[KEY_ENTER] = std::make_unique<FireCommand>(character);
    }
}

void InputHandler::handleInput() {
    if (!character) return;
    
    // Xử lý phím giữ (movement)
    for (const auto& binding : keyBindings) {
        int key = binding.first;
        Command* command = binding.second.get();
        
        // Movement commands (left/right) - continuous
        if ((key == KEY_LEFT || key == KEY_RIGHT || key == KEY_A || key == KEY_D) && 
            IsKeyDown(key)) {
            command->execute();
        }
    }
    
    // Xử lý phím nhấn một lần (jump, duck, fire)
    for (const auto& binding : keyBindings) {
        int key = binding.first;
        Command* command = binding.second.get();
        
        // Action commands - press once
        if ((key == KEY_UP || key == KEY_DOWN || key == KEY_SPACE || 
             key == KEY_W || key == KEY_S || key == KEY_ENTER) && 
            IsKeyPressed(key)) {
            command->execute();
        }
    }
    
    // Xử lý stop moving khi không nhấn phím di chuyển
    bool isMoving = false;
    if (controlType == ControlType::ARROWS) {
        isMoving = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT);
    } else {
        isMoving = IsKeyDown(KEY_A) || IsKeyDown(KEY_D);
    }
    
    if (!isMoving) {
        character->stopMoving();
    }
    
    // Reset ducking khi không nhấn phím down
    bool isDuckKeyPressed = false;
    if (controlType == ControlType::ARROWS) {
        isDuckKeyPressed = IsKeyDown(KEY_DOWN);
    } else {
        isDuckKeyPressed = IsKeyDown(KEY_S);
    }
    
    character->SetDucking(isDuckKeyPressed);
}

void InputHandler::setControlType(ControlType type) {
    controlType = type;
    if (type == ControlType::ARROWS) {
        setupArrowControls();
    } else {
        setupWASDControls();
    }
}

void InputHandler::bindKey(int key, std::unique_ptr<Command> command) {
    keyBindings[key] = std::move(command);
}