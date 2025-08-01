#pragma once

#include "raylib.h"
#include "Block.h"


class GlassBlock : public Block {

public:

    GlassBlock( Vector2 pos, Vector2 dim, Color color );
    GlassBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~GlassBlock() override;

    void doHit(Character& mario, Map* map) override; // No specific action for GlassBlock
    void Update() override;
    void Draw() override;

};