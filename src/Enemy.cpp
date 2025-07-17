#include "Enemy.h"
#include "GameWorld.h"

Enemy::Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir)
    : Object(pos, size, vel, color, friction, currFrame, dir), maxSpeedX(50.0f), textureIndex(0) {
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
    if (sprite) {
        DrawTexture(*sprite, (int)pos.x, (int)pos.y, WHITE);
    } else {
        DrawRectangle((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, color);
    }
    drawCollisionProbes();
}