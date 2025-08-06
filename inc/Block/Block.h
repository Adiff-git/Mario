
#pragma once
#include "raylib.h"
#include "Object.h"
#include "../inc/Character/Character.h"
class Map;
enum BlockType {
    BLOCK_QUESTION = 0,
    BLOCK_CLOUD = 1,
    BLOCK_WOOD = 2,
    BLOCK_GLASS = 3,
    BLOCK_EYES_OPENED = 4,
    BLOCK_EYES_CLOSED = 5
};
enum GiftType{
    GIFT_NONE,
    GIFT_COIN,
    GIFT_STAR_DUST,
    GIFT_FIRE_FLOWER,
    GIFT_POINTS
};
class Block : public Object {

protected:
    bool hit;
    BlockType blockType = BLOCK_QUESTION;
public:

    Block();
    Block( Vector2 pos, Vector2 dim, Color color );
    Block( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~Block() override;

    void Update() override = 0 ;
    void Draw() override = 0;
    virtual void doHit( Character &mario, Map* map ) = 0;
    void resetHit();
    bool isHit() const { return hit; }

    BlockType GetBlockType();
    GiftType GetGiftType() { return giftType;}
    GiftType giftType = GIFT_NONE;
};