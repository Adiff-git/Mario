#include "Item.h"
Item::Item()
    : Item({0, 0}, {0, 0}, {0, 0}, WHITE, 0, 0, 0) {
}

Item::Item(Vector2 pos, Vector2 dim, Color color, int earnedPoints)
    : Item(pos, dim, {0, 0}, color, 0, 0, earnedPoints) {
}

Item::Item(Vector2 pos, Vector2 dim, Color color, float frameTime, int maxFrames, int earnedPoints)
    : Item(pos, dim, {0, 0}, color, frameTime, maxFrames, earnedPoints) {
}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, int earnedPoints)
    : Item(pos, dim, vel, color, 0, 0, earnedPoints) {
}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float frameTime, int maxFrames)
    : Item(pos, dim, vel, color, frameTime, maxFrames, 0) {
}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color,
           float frameTime, int maxFrames, Direction facingDirection,
           int hitsToDie, int earnedPoints)
    : Sprite(pos, dim, vel, color, frameTime, maxFrames),
      onHitFrameAcum(0),
      onHitFrameTime(0.1f),
      maxOnHitFrame(1),
      currentOnHitFrame(0),
      pointsFrameAcum(0),
      pointsFrameTime(1),
      pauseGameOnHit(false) {
}

Item::~Item() = default;

void Item::onSouthCollision(Mario& mario) {
    // Mặc định: không làm gì
}

bool Item::isPauseGameOnHit() {
    return pauseGameOnHit;
}

CollisionType Item::checkCollision(Sprite* sprite) {
    if (CheckCollisionRecs(getRect(), sprite->getRect())) {
        return COLLISION_TYPE_COLLIDED;
    }
    return COLLISION_TYPE_NONE;
}