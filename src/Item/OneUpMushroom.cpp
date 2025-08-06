#include "../inc/Item/OneUpMushroom.h"
#include "../inc/World/GameWorld.h"
#include "../inc/World/GameClock.h"
#include "../inc/Character/Character.h"
#include "ResrcManager.h"

OneUpMushroom::OneUpMushroom(Vector2 pos)
    : Item(pos, {32, 32}, {40, 0}, WHITE, 0.0f, 1, DIRECTION_RIGHT, 1, 0),
      applyGravity(true),
      isMoving(true)
{
    state = OBJECT_STATE_ACTIVE;
    sprite = &ResrcManager::GetInstance().getTexture("1UpMushroom");
    
    collected = false;
    scoreDisplayTimer = 0.0f;
    scoreYOffset = 0.0f;
    scoreAlpha = 1.0f;
    scoreScale = 1.0f;
    scoreTexture = &ResrcManager::GetInstance().getTexture("+1UP");
}

void OneUpMushroom::updateMario(Character &mario)
{
    if (collected) return;

    mario.SetLives(mario.GetLives() + 1); 
    //chỗ nào có cần cộng coin hay score không?
    
    collected = true; 
    scoreDisplayTimer = 1.5f; 
    scoreYOffset = 0.0f;      
    scoreAlpha = 1.0f;       
    scoreScale = 0.5f;        
}

void OneUpMushroom::playCollisionSound()
{
    // Optional: Add sound effect
}

void OneUpMushroom::Update()
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
        const float dt = 1.0f / 60.0f;
        float fixedDt = GameClock::GetInstance().FIXED_TIME_STEP;

        if (state == OBJECT_STATE_ACTIVE || state == OBJECT_STATE_FALLING)
        {
            vel.y += GameWorld::GetGravity() * dt;

            if (vel.y > 0)
                state = OBJECT_STATE_FALLING;
            else if (vel.y < 0 && state != OBJECT_STATE_ON_GROUND)
                state = OBJECT_STATE_ACTIVE;

            pos.x += vel.x * fixedDt;
            pos.y += vel.y * dt;

            UpdateCollisionProbes();

            if (state == OBJECT_STATE_ON_GROUND)
            {
                vel.y = -200.f; // Bật lên nhẹ hơn star
                state = OBJECT_STATE_ACTIVE;
            }
        }
    }
}

void OneUpMushroom::Draw()
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
        
        if (sprite)
            DrawTextureEx(*sprite, pos, 0.0f, 1.0f, color);
    }
}