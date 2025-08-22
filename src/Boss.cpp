#include "Boss.h"
#include "../inc/World/GameWorld.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#ifndef PI
#define PI 3.14159265358979323846f
#endif

Boss::Boss(Vector2 startPos, Vector2 *player1Pos, Vector2 *player2Pos, bool isMultiplayer)
    : Enemy(startPos, {128.0f, 128.0f}, {0.0f, 0.0f}, WHITE, 0.8f, 0, DIRECTION_RIGHT),
      currentState(BossState::IDLE), player1Pos(player1Pos), player2Pos(player2Pos), isMultiplayer(isMultiplayer),
      currentTexture(nullptr), skillCurrentFrame(0), skillFrameTime(0.3f), skillFrameAccumulator(0.0f),
      skillDurationAccumulator(0.0f), detectionRange(500.0f), chaseRange(350.0f), attackRange(200.0f),
      moveSpeed(15.0f), chaseSpeedMultiplier(2.0f), attackCooldown(0.5f), attackTimer(0.0f),
      attackCount(0), maxAttacks(3), skillCooldown(12.0f), skillTimer(0.0f), isUsingSkill(false),
      hasPlayedIdleAnimation(false), patrolTimer(0.0f), patrolPhase(0), hitCount(0),
      hitCooldown(0.0f), blinkingAlpha(1.0f), fadingOut(true)
{
    frameTime = 0.35f;
    frameAcumulator = 0.0f;
    currentFrame = 0;
    maxFrames = 4;
    // std::cout << "[DEBUG] Boss initialized at pos: (" << startPos.x << ", " << startPos.y 
    //           << "), player1Pos: " << (player1Pos ? "valid" : "nullptr")
    //           << ", player2Pos: " << (player2Pos ? "valid" : "nullptr") 
    //           << ", multiplayer: " << (isMultiplayer ? "true" : "false") << std::endl;
    LoadTextures();
    UpdateTexture();
    behavior = nullptr;
    BuildBehaviorTree();
}

Boss::~Boss()
{
    CleanupProjectiles();
    if (behavior) { delete behavior; }
}

void Boss::BuildBehaviorTree()
{
    auto canSeeMario = new ConditionNode([this](){ return CanSeeMario(); });
    auto isCloseToMario = new ConditionNode([this](){ return IsCloseToMario(); });
    auto isAttackReady = new ConditionNode([this](){ return attackCount < maxAttacks; });
    auto shouldUseSkill = new ConditionNode([this](){ return attackCount >= maxAttacks && IsSkillReady(); });
    auto notInActionState = new ConditionNode([this](){ return !(currentState == BossState::SKILL && isUsingSkill); });
    auto isIdleNotPlayed = new ConditionNode([this](){ return !hasPlayedIdleAnimation; });

    auto doSkill = new ActionNode([this](){ SetState(BossState::SKILL); });
    auto doAttack = new ActionNode([this](){ SetState(BossState::ATTACK); });
    auto doChase = new ActionNode([this](){ SetState(BossState::CHASE); }); 
    auto doPatrol = new ActionNode([this](){ SetState(BossState::PATROL); });
    auto doIdle = new ActionNode([this](){ SetState(BossState::IDLE); });

    auto idleSequence = new SequenceNode({notInActionState, isIdleNotPlayed, doIdle});
    auto skillSequence = new SequenceNode({notInActionState, isCloseToMario, shouldUseSkill, doSkill});
    auto attackSequence = new SequenceNode({notInActionState, isCloseToMario, isAttackReady, doAttack});
    auto chaseSequence = new SequenceNode({notInActionState, canSeeMario, doChase});
    auto patrolAction = new SequenceNode({notInActionState, doPatrol});
    auto rootSelector = new SelectorNode({idleSequence, skillSequence, attackSequence, chaseSequence, patrolAction});
    behavior = new BehaviorTree(rootSelector);
}

void Boss::Update() 
{
    if (currentState == BossState::DIE) {
        return; // Không cập nhật các logic khác khi chết
    }
    UpdateStateAndPhysic();
}

void Boss::UpdateStateAndPhysic()
{
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    UpdateTimers(dt);
    UpdateSmoothBlinking();
    if (behavior) { behavior->Tick(); }
    UpdateMovement();
    UpdateBoundaries();
    UpdateAnimations(dt);
    UpdateTexture();
    UpdateProjectiles();
    UpdateCollisionProbes();
}

void Boss::UpdateTimers(float dt)
{
    attackTimer -= dt;
    skillTimer -= dt;
    if (skillTimer < 0) skillTimer = 0;
    hitCooldown -= dt;
    if (hitCooldown < 0) hitCooldown = 0;
    if (currentState == BossState::SKILL && !skillFlyFrames.empty()) {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime) {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
        }
    }
}

void Boss::UpdateMovement()
{
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    switch (currentState) {
        case BossState::IDLE:
            Idle(dt);
            pos.x += vel.x * dt;
            pos.y += vel.y * dt;
            break;
        case BossState::PATROL:
            Patrol(dt);
            pos.x += vel.x * dt;
            break;
        case BossState::CHASE:
            Chase(dt);
            pos.x += vel.x * dt;
            break;
        case BossState::ATTACK:
            Attack(dt);
            pos.x += vel.x * dt;
            break;
        case BossState::SKILL:
            UseSkill(dt);
            break;
        case BossState::DIE:
            // Không di chuyển khi chết 
            vel={0.0f,0.0f};
            break;
    }
}

void Boss::UpdateBoundaries()
{
    if (currentState == BossState::PATROL) {
        if (pos.x < 20) { pos.x = 20; vel.x = 0; direction = DIRECTION_RIGHT; }
        if (pos.x > 1780) { pos.x = 1780; vel.x = 0; direction = DIRECTION_LEFT; }
    }
}

void Boss::UpdateAnimations(float dt)
{
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0;
        switch (currentState) {
            case BossState::CHASE:
                if (!chaseFrames.empty()) { currentFrame = (currentFrame + 1) % chaseFrames.size(); }
                break;
            case BossState::PATROL:
                if (!movingFrames.empty()) { currentFrame = (currentFrame + 1) % movingFrames.size(); }
                break;
            case BossState::ATTACK:
                if (!attackFrames.empty()) { currentFrame = (currentFrame + 1) % attackFrames.size(); }
                break;
            case BossState::IDLE:
                break;
            case BossState::DIE:
                break;
            default:
                if (!movingFrames.empty()) { currentFrame = (currentFrame + 1) % movingFrames.size(); }
                break;
        }
    }
}

float Boss::GetDistanceToMario() const
{
    Vector2* targetPos = GetTargetPos();
    if (!targetPos) {
        // std::cout << "[DEBUG] Boss::GetDistanceToMario - targetPos is nullptr" << std::endl;
        return 999999.0f;
    }
    float bossCenterX = pos.x + size.x / 2, bossCenterY = pos.y + size.y / 2;
    float distance = sqrt((targetPos->x - bossCenterX) * (targetPos->x - bossCenterX) +
                          (targetPos->y - bossCenterY) * (targetPos->y - bossCenterY));
    return distance;
}

Vector2 Boss::GetDirectionToMario() const
{
    Vector2* targetPos = GetTargetPos();
    if (!targetPos) return {0, 0};
    float bossCenterX = pos.x + size.x / 2, bossCenterY = pos.y + size.y / 2;
    float dx = targetPos->x - bossCenterX, dy = targetPos->y - bossCenterY;
    float distance = GetDistanceToMario();
    if (distance > 0) { return {dx / distance, dy / distance}; }
    return {0, 0};
}

Vector2 Boss::GetPredictedDirectionToMario() const
{
    Vector2* targetPos = GetTargetPos();
    if (!targetPos) return {0, 0};
    float projectileSpeed = 300.0f;
    float distance = GetDistanceToMario();
    float timeToReach = distance / projectileSpeed;
    Vector2 predictedPos = *targetPos;
    float bossCenterX = pos.x + size.x / 2, bossCenterY = pos.y + size.y / 2;
    float dx = predictedPos.x - bossCenterX, dy = predictedPos.y - bossCenterY;
    float predictedDistance = sqrt(dx * dx + dy * dy);
    if (predictedDistance > 0) { return {dx / predictedDistance, dy / predictedDistance}; }
    return GetDirectionToMario();
}

Vector2* Boss::GetTargetPos() const
{
    if (!player1Pos) {
        // std::cout << "[DEBUG] Boss::GetTargetPos - player1Pos is nullptr" << std::endl;
        return nullptr;
    }
    if (!isMultiplayer || !player2Pos) {
        return player1Pos;
    }
    float bossCenterX = pos.x + size.x / 2, bossCenterY = pos.y + size.y / 2;
    float distToPlayer1 = sqrt((player1Pos->x - bossCenterX) * (player1Pos->x - bossCenterX) +
                               (player1Pos->y - bossCenterY) * (player1Pos->y - bossCenterY));
    float distToPlayer2 = sqrt((player2Pos->x - bossCenterX) * (player2Pos->x - bossCenterX) +
                               (player2Pos->y - bossCenterY) * (player2Pos->y - bossCenterY));
    return (distToPlayer1 <= distToPlayer2) ? player1Pos : player2Pos;
}

void Boss::SetMarioPosition(Vector2* player1Pos, Vector2* player2Pos, bool isMultiplayer)
{
    this->player1Pos = player1Pos;
    this->player2Pos = player2Pos;
    this->isMultiplayer = isMultiplayer;
    // std::cout << "[DEBUG] Boss::SetMarioPosition - player1Pos: " << (player1Pos ? "valid" : "nullptr")
    //           << ", player2Pos: " << (player2Pos ? "valid" : "nullptr")
    //           << ", multiplayer: " << (isMultiplayer ? "true" : "false") << std::endl;
}

void Boss::LoadTextures()
{
    try {
        ResrcManager &resrc = ResrcManager::GetInstance();
        idleFrames.clear(); movingFrames.clear(); chaseFrames.clear(); attackFrames.clear(); skillFlyFrames.clear();
        movingFrames.push_back(&resrc.getTexture("Moving 1"));
        movingFrames.push_back(&resrc.getTexture("Moving 2"));
        movingFrames.push_back(&resrc.getTexture("Moving 3"));
        movingFrames.push_back(&resrc.getTexture("Moving 4"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_1"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_2"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_3"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_4"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_5,7"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_6"));
        chaseFrames.push_back(&resrc.getTexture("Skill 1_5,7"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_1"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_2"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_3"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_4"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_1"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_2"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_3,6"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_4"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_5"));
        idleFrames.push_back(&resrc.getTexture("Waiting"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_1"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_2"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_3"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_4"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_5"));
        idleFrames.push_back(&resrc.getTexture("Skill 6_6"));

        if (!attackFrames.empty()) {
            currentTexture = attackFrames[0];
        } else if (!movingFrames.empty()) {
            currentTexture = movingFrames[0];
        } else if (!chaseFrames.empty()) {
            currentTexture = chaseFrames[0];
        } else if (!idleFrames.empty()) {
            currentTexture = idleFrames[0];
        } else {
            currentTexture = nullptr;
        }
    } catch (...) { currentTexture = nullptr; }
}

void Boss::UpdateTexture()
{
    switch (currentState) {
        case BossState::IDLE:
            if (!idleFrames.empty()) {
                if (currentFrame >= idleFrames.size()) {
                    currentFrame = 0;
                }
                currentTexture = idleFrames[currentFrame];
                sprite = currentTexture;
                Vector2* targetPos = GetTargetPos();
                if (targetPos) {
                    if (targetPos->x > pos.x) direction = DIRECTION_RIGHT;
                    else direction = DIRECTION_LEFT;
                }
            }
            break;
        case BossState::PATROL:
            if (!movingFrames.empty()) {
                if (currentFrame >= movingFrames.size()) { currentFrame = 0; }
                currentTexture = movingFrames[currentFrame];
                sprite = currentTexture;
                if (vel.x > 0) direction = DIRECTION_RIGHT;
                else if (vel.x < 0) direction = DIRECTION_LEFT;
            }
            break;
        case BossState::CHASE:
            if (!chaseFrames.empty()) {
                if (currentFrame >= chaseFrames.size()) { currentFrame = 0; }
                currentTexture = chaseFrames[currentFrame];
                if (currentTexture == nullptr) { currentFrame = 0; currentTexture = chaseFrames[0]; }
                sprite = currentTexture;
                Vector2* targetPos = GetTargetPos();
                if (targetPos) {
                    if (targetPos->x > pos.x) direction = DIRECTION_RIGHT;
                    else direction = DIRECTION_LEFT;
                }
            } else {
                if (!movingFrames.empty()) {
                    currentTexture = movingFrames[0];
                    sprite = currentTexture;
                }
            }
            break;
        case BossState::ATTACK:
            if (!attackFrames.empty()) {
                if (currentFrame >= attackFrames.size()) { currentFrame = 0; }
                currentTexture = attackFrames[currentFrame];
                sprite = currentTexture;
                Vector2* targetPos = GetTargetPos();
                if (targetPos) {
                    if (targetPos->x > pos.x) direction = DIRECTION_RIGHT;
                    else direction = DIRECTION_LEFT;
                }
            }
            break;
        case BossState::SKILL:
            if (!skillFlyFrames.empty()) {
                currentTexture = skillFlyFrames[skillCurrentFrame];
                sprite = currentTexture;
                Vector2* targetPos = GetTargetPos();
                if (targetPos) {
                    if (targetPos->x > pos.x) direction = DIRECTION_RIGHT;
                    else direction = DIRECTION_LEFT;
                }
            }
            break;
        default:
            if (!movingFrames.empty()) {
                currentTexture = movingFrames[0];
                sprite = currentTexture;
            }
            break;
    }
}

void Boss::Draw()
{   if (currentState == BossState::DIE) {
        return;
    }
    DrawSprite();
    DrawHealthBar();
    DrawDebugInfo();
    DrawProjectiles();
}

void Boss::DrawSprite()
{
    if (sprite) {
        Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
        Rectangle dest = {pos.x, pos.y, size.x, size.y};
        Vector2 origin = {0, 0};
        if (direction == DIRECTION_RIGHT) { source.width = -abs(source.width); }
        else { source.width = abs(source.width); }
        Color drawColor = color;
        if (IsBlinking()) { drawColor.a = (unsigned char)(255 * blinkingAlpha); }
        DrawTexturePro(*sprite, source, dest, origin, angle, drawColor);
    } else {
        Color rectColor = RED;
        if (IsBlinking()) { rectColor.a = (unsigned char)(255 * blinkingAlpha); }
        DrawRectangle(pos.x, pos.y, size.x, size.y, rectColor);
        Color textColor = WHITE;
        if (IsBlinking()) { textColor.a = (unsigned char)(255 * blinkingAlpha); }
        // DrawText("BOSS", pos.x + 10, pos.y + 20, 12, textColor);
    }
}

void Boss::DrawHealthBar()
{
    Texture2D *healthBarTexture = GetCurrentHealthBarTexture();
    if (healthBarTexture) {
        Rectangle healthBarSource = {0, 0, (float)healthBarTexture->width, (float)healthBarTexture->height};
        float healthBarWidth = 80.0f, healthBarHeight = 12.0f;
        float healthBarX = pos.x + (size.x - healthBarWidth) / 2, healthBarY = pos.y - 25;
        Rectangle healthBarDest = {healthBarX, healthBarY, healthBarWidth, healthBarHeight};
        Vector2 healthBarOrigin = {0, 0};
        Color healthBarColor = WHITE;
        if (IsBlinking()) { healthBarColor.a = (unsigned char)(255 * blinkingAlpha); }
        DrawTexturePro(*healthBarTexture, healthBarSource, healthBarDest, healthBarOrigin, 0.0f, healthBarColor);
    }
}

void Boss::DrawDebugInfo()
{
    // ...existing code...
    std::string frameInfo = "Frame: " + std::to_string(currentFrame);
    if (currentState == BossState::CHASE) { frameInfo += "/" + std::to_string(chaseFrames.size()) + " Chase"; }
    else if (currentState == BossState::PATROL) { frameInfo += "/" + std::to_string(movingFrames.size()) + " Patrol"; }
    float dist = GetDistanceToMario();
    // std::string debugInfo = "Dist: " + std::to_string((int)dist) + " | AtkCnt: " + std::to_string(attackCount) +
    //                        " | AtkTimer: " + std::to_string(attackTimer) + " | SkillTimer: " + std::to_string(skillTimer);
    Color textColor = YELLOW, textColor2 = GREEN, textColor3 = WHITE;
    if (IsBlinking()) {
        textColor.a = (unsigned char)(255 * blinkingAlpha);
        textColor2.a = (unsigned char)(255 * blinkingAlpha);
        textColor3.a = (unsigned char)(255 * blinkingAlpha);
    }
    // ...existing code...
}

void Boss::FireProjectile(Vector2 direction)
{
    FireType projectileType = FireType::SMALL;
    int currentAttackInCycle = attackCount % maxAttacks;
    switch (currentAttackInCycle) {
        case 0: projectileType = FireType::SMALL; break;
        case 1: projectileType = FireType::NORMAL; break;
        case 2: projectileType = FireType::BIG; break;
        default: projectileType = FireType::SMALL; break;
    }
    Vector2 projectilePos = { pos.x + size.x / 2 - 16, pos.y + size.y / 2 - 16 };
    Vector2 accurateDirection = GetPredictedDirectionToMario();
    BossFireball *projectile = new BossFireball(projectilePos, accurateDirection, projectileType);
    projectiles.push_back(projectile);
    // std::cout << "[DEBUG] Boss fired projectile at target: (" << GetTargetPos()->x << ", " << GetTargetPos()->y << ")" << std::endl;
}

void Boss::OnStateEnter(BossState newState)
{
    bool shouldResetFrame = true;
    if ((currentState == BossState::PATROL && newState == BossState::CHASE) ||
        (currentState == BossState::CHASE && newState == BossState::PATROL)) {
        shouldResetFrame = false;
    }
    if (currentState == newState) return;
    switch (newState) {
        case BossState::PATROL: patrolTimer = 0.0f; patrolPhase = 0; break;
        case BossState::CHASE: break;
        case BossState::ATTACK: vel = {0.0f, 0.0f}; break;
        case BossState::SKILL: vel = {0.0f, 0.0f}; isUsingSkill = true; skillCurrentFrame = 0;
                               skillFrameAccumulator = 0.0f; skillDurationAccumulator = 0.0f; shouldResetFrame = true; break;
        default: break;
    }
    if (shouldResetFrame) {
        currentFrame = 0;
    } else {
        switch (newState) {
            case BossState::CHASE: if (!chaseFrames.empty() && currentFrame >= chaseFrames.size()) { currentFrame = currentFrame % chaseFrames.size(); } break;
            case BossState::PATROL: if (!movingFrames.empty() && currentFrame >= movingFrames.size()) { currentFrame = currentFrame % movingFrames.size(); } break;
            case BossState::ATTACK: if (!attackFrames.empty() && currentFrame >= attackFrames.size()) { currentFrame = currentFrame % attackFrames.size(); } break;
        }
    }
}

void Boss::SetState(BossState newState)
{
    if (currentState != newState) {
        OnStateEnter(newState);
        currentState = newState;
        // std::cout << "[DEBUG] Boss state changed to: " << static_cast<int>(newState) << std::endl;
    }
}

bool Boss::CanSeeMario() const
{
    float dist = GetDistanceToMario();
    bool canSee = dist < detectionRange;
    return canSee;
}

bool Boss::IsCloseToMario() const
{
    float dist = GetDistanceToMario();
    bool isClose = dist < attackRange;
    return isClose;
}

void Boss::Chase(float dt)
{
    Vector2 directionToMario = GetDirectionToMario();
    float currentSpeed = moveSpeed * chaseSpeedMultiplier;
    const float acceleration = 80.0f, maxChaseSpeed = 50.0f, minChaseSpeed = 8.0f;
    if (directionToMario.x == 0 && directionToMario.y == 0) {
        vel.x *= 0.9f;
    } else {
        float targetVelX = directionToMario.x * currentSpeed;
        if (currentFrame >= 4) {
            targetVelX = directionToMario.x * currentSpeed * 2.5f;
        }
        vel.x += (targetVelX - vel.x) * acceleration * dt;
        float maxSpeed = (currentFrame >= 4) ? maxChaseSpeed * 2.0f : maxChaseSpeed;
        if (abs(vel.x) > maxSpeed) {
            vel.x = (vel.x > 0) ? maxSpeed : -maxSpeed;
        } else if (abs(vel.x) < minChaseSpeed && abs(vel.x) > 0) {
            vel.x = (vel.x > 0) ? minChaseSpeed : -minChaseSpeed;
        }
    }
    if (vel.x < -1.0f) { direction = DIRECTION_LEFT; }
    else if (vel.x > 1.0f) { direction = DIRECTION_RIGHT; }
}

void Boss::Attack(float dt)
{
    Vector2 directionToMario = GetPredictedDirectionToMario();
    const float attackMoveSpeed = moveSpeed * 0.3f, positioningSpeed = 15.0f;
    float distanceToMario = GetDistanceToMario();
    float optimalAttackDistance = attackRange * 0.7f;
    if (distanceToMario > optimalAttackDistance + 20.0f) {
        vel.x += directionToMario.x * positioningSpeed * dt;
        vel.y += directionToMario.y * positioningSpeed * dt;
    } else if (distanceToMario < optimalAttackDistance - 20.0f) {
        vel.x -= directionToMario.x * positioningSpeed * dt;
        vel.y -= directionToMario.y * positioningSpeed * dt;
    } else {
        Vector2 perpendicular = {-directionToMario.y, directionToMario.x};
        vel.x += perpendicular.x * attackMoveSpeed * 0.5f * dt;
        vel.y += perpendicular.y * attackMoveSpeed * 0.5f * dt;
    }
    vel.x *= 0.95f;
    vel.y *= 0.95f;
    float maxAttackSpeed = attackMoveSpeed * 1.5f;
    float currentSpeed = sqrt(vel.x * vel.x + vel.y * vel.y);
    if (currentSpeed > maxAttackSpeed) {
        vel.x = (vel.x / currentSpeed) * maxAttackSpeed;
        vel.y = (vel.y / currentSpeed) * maxAttackSpeed;
    }
    if (directionToMario.x < -0.1f) { direction = DIRECTION_LEFT; }
    else if (directionToMario.x > 0.1f) { direction = DIRECTION_RIGHT; }
    if (attackTimer <= 0) {
        FireProjectile(directionToMario);
        attackTimer = attackCooldown;
        attackCount++;
        int currentAttackInCycle = (attackCount - 1) % maxAttacks + 1;
        // std::cout << "[DEBUG] Boss fired projectile at target: (" << GetTargetPos()->x << ", " << GetTargetPos()->y << ")" << std::endl;
    }
}

void Boss::Patrol(float dt)
{
    patrolTimer += dt;
    if (patrolTimer >= 3.0f) {
        patrolTimer = 0.0f;
        patrolPhase = (patrolPhase + 1) % 4;
    }
    switch (patrolPhase) {
        case 0: vel.x = moveSpeed; direction = DIRECTION_RIGHT; break;
        case 1: vel.x = -moveSpeed; direction = DIRECTION_LEFT; break;
    }
}

void Boss::UseSkill(float dt)
{
    skillDurationAccumulator += dt;
    const float skillDuration = 2.0f;
    if (skillDurationAccumulator >= skillDuration) {
        skillDurationAccumulator = 0.0f;
        skillFrameAccumulator = 0.0f;
        isUsingSkill = false;
        skillTimer = skillCooldown;
        attackCount = 0;
        attackTimer = 0.0f;
        if (behavior) { behavior->Tick(); }
    }
}

void Boss::Idle(float dt)
{
    float distance = GetDistanceToMario();
    static bool idleAnimationPlaying = false;
    static float idleFrameAccumulator = 0.0f;
    static int idleCurrentFrame = 0;
    const float idleFrameTime = 0.55f;
    const float jumpVelocity = -150.0f;
    const float leftVelocity = -110.0f;
    static bool hasJumped = false;

    if (hasPlayedIdleAnimation) {
        SetState(BossState::PATROL);
        return;
    }

    if (distance > detectionRange * 1.5) {
        idleAnimationPlaying = false;
        idleCurrentFrame = 0;
        idleFrameAccumulator = 0.0f;
        vel.x = 0;
        vel.y = 0;
        hasJumped = false;
        currentFrame = 0;
    } else {
        if (!idleAnimationPlaying) {
            idleAnimationPlaying = true;
            idleCurrentFrame = 0;
            idleFrameAccumulator = 0.0f;
            hasJumped = false; 
        }

        idleFrameAccumulator += dt;
        if (idleFrameAccumulator >= idleFrameTime) {
            idleFrameAccumulator = 0.0f;
            idleCurrentFrame++;
            if (idleCurrentFrame >= idleFrames.size()) {
                idleCurrentFrame = 0;
                idleAnimationPlaying = false;
                hasPlayedIdleAnimation = true;
                hasJumped = false;
                SetState(BossState::PATROL);
                return;
            }
        }

        if (!hasJumped) {
            vel.y = jumpVelocity;
            vel.x = leftVelocity;
            hasJumped = true;
        } else {
            vel.y += GameWorld::GetGravity() * dt;
            vel.x = leftVelocity; 
        }

        if (vel.x < 0) {
            direction = DIRECTION_LEFT;
        }

        currentFrame = idleCurrentFrame;
    }
}

void Boss::UpdateProjectiles()
{
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        BossFireball *projectile = *it;
        if (projectile->isOutOfDistance() || projectile->GetState() == OBJECT_STATE_DEAD) {
            delete projectile;
            it = projectiles.erase(it);
        } else {
            projectile->UpdateStateAndPhysic();
            ++it;
        }
    }
}

void Boss::DrawProjectiles()
{
    for (BossFireball *projectile : projectiles) {
        if (projectile && !projectile->isOutOfDistance()) {
            projectile->Draw();
        }
    }
}

void Boss::CleanupProjectiles()
{
    for (BossFireball *projectile : projectiles) { delete projectile; }
    projectiles.clear();
}

void Boss::OnHitByFireball()
{
    if (hitCooldown > 0) return;
    hitCount++;
    hitCooldown = hitCooldownTime;
    if (hitCount >= maxHits) { UpdateDeathEffect(); 
        SetState(BossState::DIE); }
    else { Enemy::StartBlinking(1.5f, 0.1f); }
}

void Boss::UpdateSmoothBlinking()
{
    Enemy::UpdateBlinking();
    if (!IsBlinking()) { blinkingAlpha = 1.0f; return; }
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    static float blinkTime = 0.0f;
    blinkTime += dt;
    const float blinkSpeed = 6.0f;
    blinkingAlpha = 0.3f + 0.7f * (0.5f + 0.5f * sin(blinkTime * blinkSpeed));
    if (!IsBlinking()) { blinkTime = 0.0f; }
}

Texture2D* Boss::GetCurrentHealthBarTexture() const
{
    try {
        ResrcManager &resrc = ResrcManager::GetInstance();
        int healthBarFrame = (hitCount / 2) + 1;
        if (healthBarFrame > 5) healthBarFrame = 5;
        std::string healthBarName = "HealthBar" + std::to_string(healthBarFrame);
        return &resrc.getTexture(healthBarName);
    } catch (...) { return nullptr; }
}