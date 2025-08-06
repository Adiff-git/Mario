#include "../inc/Enemy/Enemy.h"
#include "../inc/World/GameWorld.h"
#include <cmath>

// Static variable definition
GameWorld* Enemy::currentGameWorld = nullptr;

Enemy::Enemy(Vector2 pos, Vector2 size, Vector2 vel, Color color, float friction, int currFrame, Direction dir)
    : Object(pos, size, vel, color, friction, currFrame, dir), maxSpeedX(50.0f), textureIndex(0),
      isBlinking(false), blinkingAcum(0), blinkingTime(0.1f), blinkingAcumTotal(0), doBlink(false),
      markedForRemoval(false), hitByFireball(false), isActive(false), activationDistance(1000.0f), deathSmoke(nullptr), scoreEffect(nullptr),
      hasDeathEffect(false),
      dyingTimer(0.0f),
      dyingDuration(1.0f)
{
    cpN.setSize(Vector2{size.x / 2, 1});
    cpS.setSize(Vector2{size.x / 2, 1});
    cpE.setSize(Vector2{5, size.y - 5});
    cpW.setSize(Vector2{5, size.y - 5});
    cpN.setColor(RED);
    cpS.setColor(RED);
    cpE.setColor(RED);
    cpW.setColor(RED);
    UpdateCollisionProbes();
}

void Enemy::Update()
{
    UpdateStateAndPhysic();
    UpdateScoreEffect(); // Cập nhật score effect
}

void Enemy::Update(Vector2 player1Pos, Vector2 player2Pos)
{
    CheckActivation(player1Pos, player2Pos);

    // Only update physics if enemy is active
    if (isActive)
    {
        UpdateStateAndPhysic();
    }
    UpdateScoreEffect(); // Cập nhật score effect
}

void Enemy::UpdateCollisionProbes()
{
    cpN.setPos(Vector2{pos.x + size.x / 2 - cpN.getSize().x / 2, pos.y});
    cpS.setPos(Vector2{pos.x + size.x / 2 - cpS.getSize().x / 2, pos.y + size.y - cpS.getSize().y + 1});
    cpE.setPos(Vector2{pos.x + size.x - cpE.getSize().x, pos.y + size.y / 2 - cpE.getSize().y / 2});
    cpW.setPos(Vector2{pos.x, pos.y + size.y / 2 - cpW.getSize().y / 2});
}

void Enemy::drawCollisionProbes()
{
    // cpN.Draw();
    // cpS.Draw();
    // cpE.Draw();
    // cpW.Draw();
}

void Enemy::Draw()
{
    UpdateBlinking();

    // Chỉ vẽ enemy sprite khi không DYING/DEAD
    if (state != OBJECT_STATE_DYING && state != OBJECT_STATE_DEAD)
    {
        if (isBlinking && doBlink)
            return;

        if (sprite)
        {
            DrawTexture(*sprite, (int)pos.x, (int)pos.y, WHITE);
        }
        else
        {
            DrawRectangle((int)pos.x, (int)pos.y, (int)size.x, (int)size.y, color);
        }
    }

    // Luôn vẽ collision probes (cho debug)
    drawCollisionProbes();

    // Vẽ death effect nếu đang DYING
    DrawDeathEffect();

    // Vẽ score effect nếu có
    DrawScoreEffect();
}

void Enemy::StartBlinking(float duration, float interval)
{
    isBlinking = true;
    blinkingAcum = 0;
    blinkingTime = interval;
    blinkingAcumTotal = 0;
    doBlink = false;
}

void Enemy::UpdateBlinking()
{
    if (!isBlinking)
        return;

    blinkingAcum += GameClock::GetInstance().FIXED_TIME_STEP;
    blinkingAcumTotal += GameClock::GetInstance().FIXED_TIME_STEP;

    if (blinkingAcum >= blinkingTime)
    {
        doBlink = !doBlink;
        blinkingAcum = 0;
    }

    if (blinkingAcumTotal >= 0.8f)
    {
        StopBlinking();
        markedForRemoval = true;
        this->SetState(OBJECT_STATE_TO_BE_REMOVED);
    }
}

bool Enemy::IsBlinking() const
{
    return isBlinking;
}

void Enemy::StopBlinking()
{
    isBlinking = false;
    blinkingAcum = 0;
    blinkingTime = 0.1f;
    blinkingAcumTotal = 0;
    doBlink = false;
}

bool Enemy::ShouldRender() const
{
    return !doBlink;
}

bool Enemy::ShouldBeRemoved() const
{
    return markedForRemoval;
}

void Enemy::CheckActivation(Vector2 player1Pos, Vector2 player2Pos)
{
    float distance = GetDistanceToNearestPlayer(player1Pos, player2Pos);

    if (distance <= activationDistance)
    {
        isActive = true;
    }
    // Once activated, enemy stays active (you can modify this behavior if needed)
}

float Enemy::GetDistanceToNearestPlayer(Vector2 player1Pos, Vector2 player2Pos) const
{
    float dist1 = GetDistanceToPlayer(player1Pos);

    // Check if player2 position is valid (not the default -1000, -1000)
    if (player2Pos.x > -999 && player2Pos.y > -999)
    {
        float dist2 = GetDistanceToPlayer(player2Pos);
        return (dist1 < dist2) ? dist1 : dist2; // Return the smaller distance
    }

    return dist1; // Only player1 is valid
}

float Enemy::GetDistanceToPlayer(Vector2 playerPos) const
{
    float dx = playerPos.x - (pos.x + size.x / 2);
    float dy = playerPos.y - (pos.y + size.y / 2);
    return sqrt(dx * dx + dy * dy);
}

void Enemy::CreateDeathEffect()
{
    if (!hasDeathEffect && !deathSmoke)
    {

        deathSmoke = new SmokeEffect(Vector2{pos.x, pos.y});
        hasDeathEffect = true;
        dyingTimer = 0.0f;
    }
}

void Enemy::UpdateDeathEffect()
{
    if (deathSmoke && hasDeathEffect)
    {
        deathSmoke->Update();
    }
}

void Enemy::UpdateDyingState()
{
    if (state == OBJECT_STATE_DYING)
    {
        dyingTimer += GameClock::GetInstance().FIXED_TIME_STEP;

        // Sau 1 giây chuyển sang DEAD
        if (dyingTimer >= dyingDuration)
        {
            state = OBJECT_STATE_DEAD;
        }
    }
}
void Enemy::DrawDeathEffect()
{
    if (state == OBJECT_STATE_DYING)
    {

        if (deathSmoke && hasDeathEffect)
        {
            deathSmoke->Draw();
        }
    }
}

bool Enemy::ShouldRemoveDeathEffect() const
{
    return hasDeathEffect && deathSmoke && deathSmoke->ShouldRemove();
}

// Score Effect Methods
void Enemy::CreateScoreEffect(int score)
{
    if (!scoreEffect)
    {
        // Tạo ScoreEffect tại vị trí của enemy
        Vector2 scorePos = {pos.x, pos.y - 10}; // Hiển thị phía trên enemy một chút
        scoreEffect = new ScoreEffect(scorePos, score);
        
        // Cộng điểm cho player ngay khi tạo animation
        if (currentGameWorld) {
            Character* player = currentGameWorld->GetPlayer1();
            if (player) {
                player->AddScore(score);
            }
        }
    }
}

void Enemy::UpdateScoreEffect()
{
    if (scoreEffect)
    {
        scoreEffect->UpdateStateAndPhysic();

        // Xóa ScoreEffect khi đã hết thời gian sống
        if (scoreEffect->ShouldRemove())
        {
            delete scoreEffect;
            scoreEffect = nullptr;
        }
    }
}

void Enemy::DrawScoreEffect()
{
    if (scoreEffect)
    {
        scoreEffect->Draw();
    }
}

bool Enemy::ShouldRemoveScoreEffect() const
{
    return scoreEffect && scoreEffect->ShouldRemove();
}