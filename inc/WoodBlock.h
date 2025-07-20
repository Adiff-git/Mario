
#pragma once

#include "raylib.h"
#include "Block.h"

class WoodBlock : public Block {

public:

    WoodBlock( Vector2 pos, Vector2 dim, Color color );
    WoodBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~WoodBlock() override;

    void doHit(Mario& mario, GameWorld* world) override {};
    void Update() override;
    void Draw() override;

};