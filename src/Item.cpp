#include "Item.h"

Item::Item()
    : Object(), pauseGameOnHit(false) {
}

Item::Item(Vector2 pos, Vector2 dim, Vector2 vel, Color color,
           float frameTime, int maxFrames, Direction facingDirection,
           int hitsToDie, int earnedPoints)
    : Object(pos, dim, vel, color, frameTime, maxFrames, facingDirection),
      pauseGameOnHit(false)
{
    
}


Item::~Item() {
}

void Item::draw() {
    if (sprite && sprite->id != 0) {
        DrawTextureEx(*sprite, pos, angle, 1.0f, color);
    }
}

void Item::SetSprite(Texture2D texture) {
    if (this->sprite == nullptr) {
        this->sprite = new Texture2D(texture);
    } else {
        *this->sprite = texture;
    }
}

void Item::SetState(ObjectState state) {
    this->state = state;
}

void Item::onSouthCollision(Mario& mario) {
}

bool Item::isPauseGameOnHit() {
    return pauseGameOnHit;
}

CollisionType Item::checkCollision(const Object& other) {
    Rectangle rect1 = { pos.x, pos.y, size.x, size.y };
    Rectangle rect2 = { other.GetPos().x, other.GetPos().y, other.GetSize().x, other.GetSize().y };

    if (CheckCollisionRecs(rect1, rect2)) {
        return static_cast<CollisionType>(COLLISION_TYPE_COLLIDED);
    } else {
        return static_cast<CollisionType>(COLLISION_TYPE_NONE);
    }
}


