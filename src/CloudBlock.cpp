
#include "Block.h"
#include "CloudBlock.h"
#include "GameWorld.h"
#include "raylib.h"
#include "ResrcManager.h"
#include <iostream>

CloudBlock::CloudBlock( Vector2 pos, Vector2 size, Color color ) :
    CloudBlock( pos, size, color, 0, 1 ) {}

CloudBlock::CloudBlock( Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames ) :
    Block( pos, size, color, frameTime, maxFrames ) {
    this->blockType = BLOCK_CLOUD;
    }

CloudBlock::~CloudBlock() = default;

void CloudBlock::Update() {
}

void CloudBlock::Draw() {
    DrawTexture(
        ResrcManager::GetInstance().getTexture("BLOCK_CLOUD"),
        this->pos.x,
        this->pos.y,
        WHITE
    );
}



