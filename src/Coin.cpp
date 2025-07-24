#include "Coin.h"
#include "ResrcManager.h"
#include "Mario.h"
#include "raylib.h"

Coin::Coin(Vector2 pos)
    : Item(pos, {20, 20}, {0, 0}, YELLOW, 0.25f, 4, DIRECTION_RIGHT, 1, 100),
      collected(false), vel({0, 0}), origin(pos),
      riseAmount(0.0f), maxRise(30.0f), showScore(false)
{
    for (int i = 0; i < 4; ++i) {
        std::string name = "COIN_" + std::to_string(i);
        Texture2D* tex = &ResrcManager::GetInstance().getTexture(name);
        if (tex->id == 0) {
            TraceLog(LOG_WARNING, "Texture %s has invalid ID!", name.c_str());
        }
        coinFrames.push_back(tex);
    }

    // Load score texture
    scoreTexture = &ResrcManager::GetInstance().getTexture("+200");

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

    if(collected){
        float dt = GameClock::GetInstance().FIXED_TIME_STEP;
        vel.y = -60.0f; // velocity bay
        pos.y += vel.y * dt;
        riseAmount += -vel.y * dt;

        if (riseAmount >= maxRise) {
            this->SetState(OBJECT_STATE_TO_BE_REMOVED);
        }
    }

    UpdateCollisionProbes();
}

void Coin::Draw()
{
    if (sprite != nullptr && sprite->id != 0) {
        DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
    }

    //animation +marks
    if (collected && riseAmount < maxRise && showScore) {
        if (scoreTexture != nullptr && scoreTexture->id != 0) {
            DrawTextureEx(*scoreTexture, {pos.x, pos.y - 15}, 0.0f, 1.0f, WHITE);
        }
    }
}

void Coin::updateMario(Mario& mario)
{
    if (collected || state == OBJECT_STATE_TO_BE_REMOVED) return;

    int currentCoins = mario.GetCoins() + 200;
    if (currentCoins >= 10000) {
        mario.SetLives(mario.GetLives() + 1);
        mario.SetCoins(currentCoins - 10000);
    } else {
        mario.SetCoins(currentCoins);
    }

    collected = true;
    vel = {0, -60}; 
    showScore = true;
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void Coin::playCollisionSound()
{
    // Gọi âm thanh nếu có
}