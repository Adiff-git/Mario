#include "FireFlower.h"
#include "raylib.h"

FireFlower::FireFlower(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 0.3f, 2, DIRECTION_RIGHT, 1, 0),
      blinking(false), blinkingAcum(0), blinkingTime(0.1f), doBlink(false)
{
    this->state = OBJECT_STATE_ACTIVE;
}

void FireFlower::updateMario(Mario& mario)
{
    mario.changetoFire();
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void FireFlower::playCollisionSound()
{
    
}

void FireFlower::Update()
{
    float delta = GetFrameTime();
    frameAcumulator += delta;
    if (frameAcumulator >= frameTime)
    {
        currentFrame = (currentFrame + 1) % maxFrames;
        frameAcumulator = 0.0f;
    }

    if (blinking)
    {
        blinkingAcum += delta;
        if (blinkingAcum >= blinkingTime)
        {
            blinkingAcum = 0;
            doBlink = !doBlink;
        }
    }

    UpdateCollisionProbes();
}

void FireFlower::draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (blinking && doBlink)
        return;

    std::string texName = (currentFrame == 0) ? "FireFlower_0" : "FireFlower_1";
    Texture2D& tex = ResrcManager::GetInstance().getTexture(texName);

    DrawTextureEx(
        tex,
        pos,
        0.0f,
        1.0f,
        color
    );
}
