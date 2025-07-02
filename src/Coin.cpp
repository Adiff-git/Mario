#include "Coin.h"
#include "raylib.h"

Coin::Coin(Vector2 pos)
    : Item(pos, {20, 20}, {0, 0}, YELLOW, 0.2f, 4, DIRECTION_RIGHT, 1, 100)
{
    for (int i = 0; i < 4; ++i) {
        std::string name = "Coin_" + std::to_string(i);
        coinFrames.push_back(&ResrcManager::GetInstance().getTexture(name));
    }
    this->sprite = coinFrames[0];
    this->state = OBJECT_STATE_ACTIVE;
}

void Coin::updateMario(Mario& mario)
{
    int currentCoins = mario.GetCoins() + 1;
    mario.SetCoins(currentCoins);
    
    if (currentCoins % 100 == 0) {
        mario.SetLives(mario.GetLives() + 1);
    }

    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}


void Coin::playCollisionSound()
{
    //sound
}

void Coin::Update()
{
    frameAcumulator += GetFrameTime();
    if (frameAcumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcumulator = 0;
    }
    this->sprite = coinFrames[currentFrame];

    UpdateCollisionProbes();
}

void Coin::draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    DrawTextureEx(
        *sprite,
        pos,
        0.0f,
        1.0f,
        color
    );
}