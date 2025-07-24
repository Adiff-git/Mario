
#include "Block.h"
#include "GameWorld.h"
#include "GlassBlock.h"
#include "raylib.h"
#include "ResrcManager.h"
#include <iostream>


GlassBlock::GlassBlock( Vector2 pos, Vector2 dim, Color color ) :
    GlassBlock( pos, dim, color, 0, 1 ) {}

GlassBlock::GlassBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames ) :
    Block( pos, dim, color, frameTime, maxFrames ) {
        this-> blockType = BLOCK_GLASS;
    }

GlassBlock::~GlassBlock() = default;

void GlassBlock::Update() {}

void GlassBlock::Draw() {
    if(this->GetState() == OBJECT_STATE_TRANSITIONING_1) {
        DrawTexture(ResrcManager::GetInstance().getTexture("BLOCK_GLASS_1"), pos.x, pos.y, WHITE);
        return;
    }else if(this->GetState() == OBJECT_STATE_TRANSITIONING_2) {
        DrawTexture(ResrcManager::GetInstance().getTexture("BLOCK_GLASS_2"), pos.x, pos.y, WHITE);
        return;
    }
    DrawTexture( ResrcManager::GetInstance().getTexture("BLOCK_GLASS"), pos.x, pos.y, WHITE );
}

void GlassBlock::doHit(Mario& mario, Map* map) {
    if(this->GetState() == OBJECT_STATE_TRANSITIONING_1) this->SetState(OBJECT_STATE_TRANSITIONING_2);
    else if(this->GetState() == OBJECT_STATE_TRANSITIONING_2) this->SetState(OBJECT_STATE_TO_BE_REMOVED);
    else this->SetState(OBJECT_STATE_TRANSITIONING_1);
}