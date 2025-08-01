#include "../inc/Item/ThreeUpMoon.h"
#include "../inc/World/GameWorld.h"
#include "../inc/World/GameClock.h"
#include "../inc/Character/Character.h"
#include "raylib.h"

ThreeUpMoon::ThreeUpMoon(Vector2 pos)
    : Item(pos, {32, 32}, {100, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 3, 0)
{
    state = OBJECT_STATE_ACTIVE;
    
    // Khởi tạo hiệu ứng
    collected = false;
    scoreDisplayTimer = 0.0f;
    scoreYOffset = 0.0f;
    scoreAlpha = 1.0f;
    scoreScale = 1.0f;
    scoreTexture = &ResrcManager::GetInstance().getTexture("+3UP");
}

void ThreeUpMoon::updateMario(Character& mario)
{
    
    if (collected) return;

    mario.SetLives(mario.GetLives() + 3);     
    
    collected = true; 
    scoreDisplayTimer = 1.5f; 
    scoreYOffset = 0.0f;     
    scoreAlpha = 1.0f;       
    scoreScale = 0.5f;        
}


void ThreeUpMoon::playCollisionSound()
{
    
}

void ThreeUpMoon::Update()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    if (collected) {
        scoreDisplayTimer -= GameClock::GetInstance().FIXED_TIME_STEP;
    
        float progress = 1.0f - (scoreDisplayTimer / 1.5f);
        
        scoreYOffset = progress * -50.0f; 

        if (progress <= 0.3f) {
            scoreScale = 0.5f + (progress / 0.3f) * 0.7f; 
        } else if (progress <= 0.6f) {
            scoreScale = 1.2f - ((progress - 0.3f) / 0.3f) * 0.2f; 
        } else {
            scoreScale = 1.0f;
        }
        
        if (progress >= 0.7f) {
            float fadeProgress = (progress - 0.7f) / 0.3f;
            scoreAlpha = 1.0f - fadeProgress;
        } else {
            scoreAlpha = 1.0f;
        }
        
        if (scoreDisplayTimer <= 0) {
            this->SetState(OBJECT_STATE_TO_BE_REMOVED);
            return;
        }
    }

    if (!collected) {
        float dt = GetFrameTime();

        pos.x += vel.x * dt;
        if (applyGravity)
            vel.y += GameWorld::GetGravity() * dt;

        pos.y += vel.y * dt;

        UpdateCollisionProbes();
    }
}

void ThreeUpMoon::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (collected) {
        if (scoreTexture != nullptr && scoreTexture->id != 0) {
            
            Vector2 scorePos = {pos.x, pos.y - 15 + scoreYOffset};
            
            Color scoreColor = {255, 255, 255, (unsigned char)(scoreAlpha * 255)};
            
            DrawTextureEx(*scoreTexture, scorePos, 0.0f, scoreScale, scoreColor);
        }
    } else {
        Texture2D& tex = ResrcManager::GetInstance().getTexture("3UpMoon");
        DrawTextureEx(
            tex,
            pos,
            0.0f,
            1.0f,
            color
        );
    }
}