#include "CharacterCommands.h"
#include "Character.h"

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
    return character && character->GetState() == OBJECT_STATE_ON_GROUND && 
           character->GetMarioState() != SMALL;
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