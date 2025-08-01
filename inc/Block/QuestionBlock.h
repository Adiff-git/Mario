
#pragma once
#include "ResrcManager.h"
#include "raylib.h"
#include "Block.h"
#include "../inc/Character/Character.h"
#include "../inc/World/Map.h"

class QuestionBlock : public Block {

    float coinAnimationTime;
    float coinAnimationAcum;
    float coinFrameAcum;
    int coinAnimationFrame;
    bool coinAnimationRunning;
    float coinY;
    float coinVelY;

    float stardustAnimationTime;
    float stardustAnimationAcum;
    int stardustAnimationFrame;
    int maxStartDustAnimationFrame;
    bool stardustAnimationRunning;

    float pointsFrameAcum;
    float pointsFrameTime;
    bool pointsAnimationRunning;
    GiftType giftType = GIFT_NONE;

public:

    QuestionBlock( Vector2 pos, Vector2 dim, Color color, GiftType giftType);
    QuestionBlock( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, GiftType giftType);
    ~QuestionBlock() override;

    void Update() override;
    void Draw() override;
    void doHit( Character& mario, Map* map );
    bool isQuestionBlock()  { return true; } // Override to indicate this is a QuestionBlock
    GiftType GetGiftType() const { return giftType; }
};