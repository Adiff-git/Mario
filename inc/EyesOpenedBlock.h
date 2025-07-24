
#pragma once

#include "raylib.h"
#include "Block.h"
#include "Mario.h"

class EyesOpenedBlock : public Block {

private:
    float animationTime;
    float animationAcum;
    int roundcounter = 0;
public:

    EyesOpenedBlock( Vector2 pos, Vector2 dim, Color color );
    EyesOpenedBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~EyesOpenedBlock() override;

    void Update() override;
    void Draw() override;
    void doHit( Mario& mario, Map* map ) override;
};