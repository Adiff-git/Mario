#include "Sprite.h"

Sprite::Sprite()
    : pos({0, 0}),
      dim({0, 0}),
      vel({0, 0}),
      color(WHITE),
      frameTime(0),
      frameAcum(0),
      currentFrame(0),
      maxFrames(0),
      earnedPoints(0),
      state(SPRITE_STATE_IDLE)
{
    cpN.setColor(RED);
    cpS.setColor(GREEN);
    cpE.setColor(BLUE);
    cpW.setColor(YELLOW);
}

Sprite::Sprite(Vector2 pos, Vector2 dim, Vector2 vel, Color color,
               float frameTime, int maxFrames, Direction facingDirection,
               int hitsToDie, int earnedPoints)
    : pos(pos),
      dim(dim),
      vel(vel),
      color(color),
      frameTime(frameTime),
      frameAcum(0),
      currentFrame(0),
      maxFrames(maxFrames),
      earnedPoints(earnedPoints),
      state(SPRITE_STATE_IDLE) {
    
    cpN.setColor(RED);
    cpS.setColor(GREEN);
    cpE.setColor(BLUE);
    cpW.setColor(YELLOW);
}

void Sprite::setPos(Vector2 pos) {
    this->pos = pos;
}

void Sprite::setVel(Vector2 vel) {
    this->vel = vel;
}

Vector2& Sprite::getPos() {
    return pos;
}

Vector2& Sprite::getVel() {
    return vel;
}

Vector2& Sprite::getDim() {
    return dim;
}

Color& Sprite::getColor() {
    return color;
}

Rectangle Sprite::getRect() const {
    return { pos.x, pos.y, dim.x, dim.y };
}

SpriteState Sprite::getState() const {
    return state;
}

void Sprite::setState(SpriteState state) {
    this->state = state;
}

void Sprite::updateCollisionProbes() {
    cpN.setX(pos.x + dim.x / 2 - cpN.getWidth() / 2);
    cpN.setY(pos.y);

    cpS.setX(pos.x + dim.x / 2 - cpS.getWidth() / 2);
    cpS.setY(pos.y + dim.y - cpS.getHeight());

    cpE.setX(pos.x + dim.x - cpE.getWidth());
    cpE.setY(pos.y + dim.y / 2 - cpE.getHeight() / 2);

    cpW.setX(pos.x);
    cpW.setY(pos.y + dim.y / 2 - cpW.getHeight() / 2);
}

CollisionType Sprite::checkCollision(Sprite* other) {
    if (CheckCollisionRecs(getRect(), other->getRect())) {
        return static_cast<CollisionType>(COLLISION_TYPE_COLLIDED);
    }
    return static_cast<CollisionType>(COLLISION_TYPE_NONE);
}
