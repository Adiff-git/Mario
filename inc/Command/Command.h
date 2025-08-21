#pragma once
#include "raylib.h"

// Base Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual bool canExecute() = 0; 
};

// Null Object Pattern cho trường hợp không có command
class NullCommand : public Command {
public:
    void execute() override {}
    bool canExecute() override { return false; }
};