#pragma once
#include "Mario.h"
#include "CollisionProbe.h"
#include "Sprite.h"

class Item : public Sprite {
protected:
    float onHitFrameAcum;
    float onHitFrameTime;
    int maxOnHitFrame;
    int currentOnHitFrame;

    float pointsFrameAcum;
    float pointsFrameTime;

    bool pauseGameOnHit;

public:
    Item();
    Item(Vector2 pos, Vector2 dim, Color color, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, int earnedPoints);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames);
    Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames, int earnedPoints);

    virtual ~Item();

    virtual void onSouthCollision(Mario& mario);  // xử lý va chạm phía dưới
    virtual bool isPauseGameOnHit();              // trả về true nếu dừng game khi va chạm
    virtual void updateMario(Mario& mario) = 0;   // mỗi Item xử lý ảnh hưởng riêng tới Mario
    virtual void playCollisionSound() = 0;        // mỗi Item có âm thanh riêng

    virtual CollisionType checkCollision(Sprite* sprite); // kiểm tra va chạm
};
