
#pragma once

class Map;

#include "raylib.h"
#include "Object.h"
#include "Mario.h"

class Block : public Object {

protected:
    bool hit;

public:

    Block();
    Block( Vector2 pos, Vector2 dim, Color color );
    Block( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~Block() override;

    void Update() override = 0 ;
    void draw() override = 0;
    virtual void doHit( Mario &mario, Map *map );
    void resetHit();

};