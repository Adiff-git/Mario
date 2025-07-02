#include "CourseClearToken.h"
#include "raylib.h"

CourseClearToken::CourseClearToken(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      rotationAngle(0.0f),
      rotationSpeed(180.0f) 
{
    this->sprite = &ResrcManager::GetInstance().getTexture("CourseClearToken");
    this->state = OBJECT_STATE_ACTIVE;
}

void CourseClearToken::updateMario(Mario& mario)
{
    mario.SetState(OBJECT_STATE_TO_BE_REMOVED); 
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void CourseClearToken::playCollisionSound()
{
    //sound
}

void CourseClearToken::Update()
{
    UpdateCollisionProbes();
    rotationAngle += rotationSpeed * GetFrameTime();
    if (rotationAngle >= 360.0f)
        rotationAngle -= 360.0f;
}

void CourseClearToken::draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    DrawTexturePro(
        *sprite,
        Rectangle{ 0, 0, (float)sprite->width, (float)sprite->height },
        Rectangle{ pos.x + size.x / 2, pos.y + size.y / 2, size.x, size.y },
        Vector2{ size.x / 2, size.y / 2 }, 
        rotationAngle,
        color
    );
}
