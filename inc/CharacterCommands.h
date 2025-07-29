#pragma once
#include "Command.h"

class Character;

// Move Commands
class MoveLeftCommand : public Command {
private:
    Character* character;
public:
    MoveLeftCommand(Character* character) : character(character) {}
    void execute() override;
    bool canExecute() override;
};

class MoveRightCommand : public Command {
private:
    Character* character;
public:
    MoveRightCommand(Character* character) : character(character) {}
    void execute() override;
    bool canExecute() override;
};

class JumpCommand : public Command {
private:
    Character* character;
public:
    JumpCommand(Character* character) : character(character) {}
    void execute() override;
    bool canExecute() override;
};

class DuckCommand : public Command {
private:
    Character* character;
public:
    DuckCommand(Character* character) : character(character) {}
    void execute() override;
    bool canExecute() override;
};

class FireCommand : public Command {
private:
    Character* character;
public:
    FireCommand(Character* character) : character(character) {}
    void execute() override;
    bool canExecute() override;
};