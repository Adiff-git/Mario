#include "FireFlower.h"
#include "raylib.h"
#include "ResrcManager.h"

FireFlower::FireFlower(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 2.0f, 2, DIRECTION_RIGHT, 1, 0),
      blinking(false), blinkingAcum(0), blinkingTime(0.1f), doBlink(false), blinkingAcumTotal(0)
{
    state = OBJECT_STATE_ACTIVE;
}

void FireFlower::updateMario(Mario& mario)
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || blinking) return;

    mario.changetoFire();                      
    blinking = true;                          
    blinkingAcum = 0;
    blinkingAcumTotal = 0;
}


void FireFlower::playCollisionSound()
{
    // TODO: play sound
}

void FireFlower::Update()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    if (blinking) {
        blinkingAcum += GameClock::GetInstance().FIXED_TIME_STEP;
        blinkingAcumTotal += GameClock::GetInstance().FIXED_TIME_STEP;
        
        if (blinkingAcum >= blinkingTime) {
            doBlink = !doBlink;
            blinkingAcum = 0;
        }
        
        if (blinkingAcumTotal >= 0.8f) {
            this->SetState(OBJECT_STATE_TO_BE_REMOVED);
            return;
        }
    }

    const float animSpeed = 0.6f; 
    static float animTime = 0.0f;
    animTime += GameClock::GetInstance().FIXED_TIME_STEP;

    if (animTime >= animSpeed) {
        currentFrame = (currentFrame + 1) % maxFrames;
        animTime = 0.0f;
    }

    UpdateCollisionProbes();
}

void FireFlower::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    if (blinking && doBlink) return;

    std::string texKey = "FIRE FLOWER_" + std::to_string(currentFrame);
    Texture2D& tex = ResrcManager::GetInstance().getTexture(texKey);

    if (tex.id == 0) {
        TraceLog(LOG_ERROR, "Texture %s not loaded!", texKey.c_str());
        return;
    }

    DrawTextureEx(tex, pos, 0.0f, 1.0f, color);
}