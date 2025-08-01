#include "../inc/Item/FireFlower.h"
#include "raylib.h"
#include "ResrcManager.h"

FireFlower::FireFlower(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 1.0f, 2, DIRECTION_RIGHT, 1, 0),  
      blinking(false), blinkingAcum(0), blinkingTime(0.1f), doBlink(false), blinkingAcumTotal(0)
{
    state = OBJECT_STATE_ACTIVE;
    collected = false;
    scoreAlpha = 1.0f;
    scoreDisplayTimer = 0.0f;
    scoreYOffset = 0.0f;
    scoreScale = 1.0f;
    frameAcumulator = 0.0f;  
    scoreTexture = &ResrcManager::GetInstance().getTexture("+1000");

}

void FireFlower::updateMario(Character& mario)
{
    
    if (collected) return;

    mario.SetCoins(mario.GetCoins() + 1);
    mario.SetScore(mario.GetScore() + 1000);
    mario.changetoFire();                      
    
    collected = true; 
    scoreDisplayTimer = 1.5f; 
    scoreYOffset = 0.0f;      
    scoreAlpha = 1.0f;       
    scoreScale = 0.5f;        
}


void FireFlower::playCollisionSound()
{
    // TODO: play sound
}

void FireFlower::Update()
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

    frameAcumulator += GameClock::GetInstance().FIXED_TIME_STEP;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0;
        currentFrame = (currentFrame + 1) % maxFrames;
    }

    UpdateCollisionProbes();
}

void FireFlower::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED) return;

    if (collected) {
        
        if (scoreTexture != nullptr && scoreTexture->id != 0) {
            
            Vector2 scorePos = {pos.x, pos.y - 15 + scoreYOffset};
            
            
            Color scoreColor = {255, 255, 255, (unsigned char)(scoreAlpha * 255)};
            
            
            DrawTextureEx(*scoreTexture, scorePos, 0.0f, scoreScale, scoreColor);
        }
    } else {
        
        std::string texKey = "FIRE FLOWER_" + std::to_string(currentFrame);
        Texture2D& tex = ResrcManager::GetInstance().getTexture(texKey);
    
        if (tex.id == 0) {
            TraceLog(LOG_ERROR, "Texture %s not loaded!", texKey.c_str());
            return;
        }
    
        DrawTextureEx(tex, pos, 0.0f, 1.0f, color);
    }
}