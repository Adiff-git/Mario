#include "../inc/Command/CharacterCommands.h"
#include "../inc/Character/Character.h"
#include <iostream>
#include <cmath>
// MoveLeftCommand
void MoveLeftCommand::execute() {
    if (canExecute()) {
        character->moveLeft();
    }
}

bool MoveLeftCommand::canExecute() {
    return character && character->GetState() != OBJECT_STATE_DYING && 
           character->GetState() != OBJECT_STATE_DEAD;
}

// MoveRightCommand
void MoveRightCommand::execute() {
    if (canExecute()) {
        character->moveRight();
    }
}

bool MoveRightCommand::canExecute() {
    return character && character->GetState() != OBJECT_STATE_DYING && 
           character->GetState() != OBJECT_STATE_DEAD;
}

// JumpCommand
void JumpCommand::execute() {
    if (canExecute()) {
        character->jump();
    }
}

bool JumpCommand::canExecute() {
    return character && character->GetState() == OBJECT_STATE_ON_GROUND;
}

// DuckCommand
void DuckCommand::execute() {
    if (canExecute()) {
        character->Duck();
    }
}

bool DuckCommand::canExecute() {
    if (!character) return false;
    if (character->GetState() != OBJECT_STATE_ON_GROUND) return false;
    if (character->GetMarioState() == SMALL) return false;
    // Thêm kiểm tra vận tốc đứng yên theo trục Y
    return std::fabs(character->GetVel().y) < 0.01f;
}

// FireCommand
void FireCommand::execute() {
    if (canExecute()) {
        character->fire();
    }
}

bool FireCommand::canExecute() {
    return character && character->GetMarioState() == FIRE;
}