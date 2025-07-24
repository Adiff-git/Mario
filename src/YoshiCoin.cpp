#include "YoshiCoin.h"
#include "GameWorld.h"
#include "Mario.h"
#include "raylib.h"

YoshiCoin::YoshiCoin(Vector2 pos)
    : Item(pos, {32, 32}, {0, 0}, WHITE, 0.4f, 4, DIRECTION_RIGHT, 0, 0)
{
    collected = false;
    scoreDisplayTimer = 0.0f;
    scoreYOffset = 0.0f;
    scoreAlpha = 1.0f;
    scoreScale = 1.0f;
    state = OBJECT_STATE_ACTIVE;
    scoreTexture = &ResrcManager::GetInstance().getTexture("+2000");
}

void YoshiCoin::updateMario(Mario& mario)
{
    if (collected) return;
    
    mario.SetCoins(mario.GetCoins() + 1); 
    mario.SetScore(mario.GetScore() + 2000); 
    collected = true; 
    scoreDisplayTimer = 1.5f; 
    scoreYOffset = 0.0f;      
    scoreAlpha = 1.0f;// nhạt dần
    scoreScale = 0.5f;// từ nhỏ to dần
    this->SetState(OBJECT_STATE_TO_BE_REMOVED);
}

void YoshiCoin::playCollisionSound()
{
    
}

void YoshiCoin::Update()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (collected) {
        scoreDisplayTimer -= GameClock::GetInstance().FIXED_TIME_STEP;
        
        float progress = 1.0f - (scoreDisplayTimer / 2.0f);// càng nhỏ càng nhanh
        
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

void YoshiCoin::Draw()
{
    if (state == OBJECT_STATE_TO_BE_REMOVED || state == OBJECT_STATE_DEAD)
        return;

    if (collected) {
        if (scoreTexture != nullptr && scoreTexture->id != 0) {
            
            Vector2 scorePos = {pos.x, pos.y - 15 + scoreYOffset};
            
            Color scoreColor = {255, 255, 255, (unsigned char)(scoreAlpha * 255)};
            
            DrawTextureEx(*scoreTexture, scorePos, 0.0f, scoreScale, scoreColor);
        }
    }
    else {
        Texture2D& tex = ResrcManager::GetInstance().getTexture(
            TextFormat("YoshiCoin_%d", currentFrame)
        );

        DrawTextureEx(
            tex,
            pos,
            0.0f,
            1.0f,
            color
        );
    }
}