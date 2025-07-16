#include "Coin.h"
#include "ResrcManager.h"
#include "Mario.h"
#include "raylib.h"

Coin::Coin(Vector2 pos)
    : Item(pos, {20, 20}, {0, 0}, YELLOW, 0.25f, 4, DIRECTION_RIGHT, 1, 100),
      collected(false)
{
    for (int i = 0; i < 4; ++i) {
        std::string name = "COIN_" + std::to_string(i);
        Texture2D* tex = &ResrcManager::GetInstance().getTexture(name);
        if (tex->id == 0) {
            TraceLog(LOG_WARNING, "Texture %s has invalid ID!", name.c_str());
        }
        coinFrames.push_back(tex);
    }

    currentFrame = 0;
    frameAcumulator = 0.0f; 
    sprite = coinFrames[currentFrame];
    state = OBJECT_STATE_ACTIVE;
}


void Coin::Update() {
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD) return;

    frameAcumulator += GameClock::GetInstance().FIXED_TIME_STEP;

    if (frameAcumulator >= frameTime) {
        currentFrame = (currentFrame + 1) % maxFrames;
        sprite = coinFrames[currentFrame];
        frameAcumulator = 0;
    }

    UpdateCollisionProbes();
}

void Coin::draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (sprite != nullptr && sprite->id != 0) {
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
    }
}

void Coin::updateMario(Mario& mario)
{
    if (collected || state == OBJECT_STATE_TO_BE_REMOVED) return;

    int currentCoins = mario.GetCoins() + 1;
    if (currentCoins >= 100) {
        mario.SetLives(mario.GetLives() + 1);
        mario.SetCoins(currentCoins - 100);
    } else {
        mario.SetCoins(currentCoins);
    }

    collected = true;
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Coin::playCollisionSound()
{
    // Gọi âm thanh nếu có
}
