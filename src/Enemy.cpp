#include "Enemy.h"
#include "GameWorld.h"
#include <cmath>

Enemy::Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir)
    : Object(pos, size, vel, color, friction, currFrame, dir), maxSpeedX(50.0f), textureIndex(0),
      isBlinking(false), blinkingAcum(0), blinkingTime(0.1f), blinkingAcumTotal(0), doBlink(false), markedForRemoval(false), hitByFireball(false) {
    cpN.setSize(Vector2{size.x/2, 1});
    cpS.setSize(Vector2{size.x/2, 1});
    cpE.setSize(Vector2{5, size.y - 5});
    cpW.setSize(Vector2{5, size.y - 5});
    cpN.setColor(RED);
    cpS.setColor(RED);
    cpE.setColor(RED);
    cpW.setColor(RED);
    UpdateCollisionProbes();
}

void Enemy::Update() {
    UpdateStateAndPhysic();
}

void Enemy::UpdateCollisionProbes() {
    cpN.setPos(Vector2{pos.x + size.x / 2 - cpN.getSize().x/2, pos.y});
    cpS.setPos(Vector2{pos.x + size.x / 2 - cpS.getSize().x/2, pos.y + size.y - cpS.getSize().y + 1});
    cpE.setPos(Vector2{pos.x + size.x - cpE.getSize().x, pos.y + size.y / 2 - cpE.getSize().y / 2});
    cpW.setPos(Vector2{pos.x, pos.y + size.y / 2 - cpW.getSize().y / 2});
}

void Enemy::drawCollisionProbes() {
    cpN.Draw();
    cpS.Draw();
    cpE.Draw();
    cpW.Draw();
}

void Enemy::Draw() {
    UpdateBlinking();

    if (isBlinking && doBlink) return;

    if (sprite) {
        DrawTexture(*sprite, (int)pos.x, (int)pos.y, WHITE);
    } else {
        DrawRectangle((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, color);
    }
    
    drawCollisionProbes();
}

void Enemy::StartBlinking(float duration, float interval) {
    isBlinking = true;
    blinkingAcum = 0;
    blinkingTime = interval;
    blinkingAcumTotal = 0;
    doBlink = false;
}

void Enemy::UpdateBlinking() {
    if (!isBlinking) return;

    blinkingAcum += GameClock::GetInstance().FIXED_TIME_STEP;
    blinkingAcumTotal += GameClock::GetInstance().FIXED_TIME_STEP;
    
    if (blinkingAcum >= blinkingTime) {
        doBlink = !doBlink;
        blinkingAcum = 0;
    }
    
    if (blinkingAcumTotal >= 0.8f) {
        StopBlinking();
        markedForRemoval = true;
        this->SetState(OBJECT_STATE_TO_BE_REMOVED);
    }
}

bool Enemy::IsBlinking() const {
    return isBlinking;
}

void Enemy::StopBlinking() {
    isBlinking = false;
    blinkingAcum = 0;
    blinkingTime = 0.1f;
    blinkingAcumTotal = 0;
    doBlink = false;
}

bool Enemy::ShouldRender() const {
    return !doBlink;
}

bool Enemy::ShouldBeRemoved() const {
    return markedForRemoval;
}