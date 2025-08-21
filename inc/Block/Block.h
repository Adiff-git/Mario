
#pragma once
#include "raylib.h"
#include "Object.h"
#include "../inc/Character/Character.h"
#include "../inc/Block/BlockType.h"
class Map;
enum GiftType{//đsv
    GIFT_NONE,
    GIFT_COIN,
    GIFT_STAR_DUST,
    GIFT_FIRE_FLOWER,
    GIFT_POINTS
};
class Block : public Object {

protected://fds
    bool hit;
    BlockType blockType =  BlockType::BLOCK_QUESTION;
public:
//
  

    Block();
    Block( Vector2 pos, Vector2 dim, Color color );
    Block( Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames );
    ~Block() override;

    void Update() override = 0 ;
    void Draw() override = 0;
    virtual void doHit( Character &mario, Map* map ) = 0;
    void resetHit();
    bool isHit() const { return hit; }
    void SetHit(bool h){ hit = h; }

    BlockType GetBlockType();
    GiftType GetGiftType() { return giftType;}
    GiftType giftType = GIFT_NONE;
};