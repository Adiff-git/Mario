#include "Boss.h"
#include <cmath>
#include <iostream>
#include <algorithm> // for std::min

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Boss::Boss(Vector2 startPos, Vector2 *marioPosition)
    : Enemy(startPos, {128.0f, 128.0f}, {0.0f, 0.0f}, WHITE, 0.8f, 0, DIRECTION_RIGHT),
      currentState(BossState::PATROL), marioPos(marioPosition),
      detectionRange(500.0f), chaseRange(350.0f), attackRange(200.0f),
      moveSpeed(15.0f), chaseSpeedMultiplier(2.0f),
      attackCooldown(0.5f), attackTimer(0.0f), // Giảm từ 1.5f xuống 0.5f
      attackCount(0), maxAttacks(3), skillCooldown(12.0f), skillTimer(0.0f), // Increased from 8.0f to 12.0f
      isUsingSkill(false), skillCurrentFrame(0), skillFrameTime(0.3f), skillFrameAccumulator(0.0f), // Slower skill frame time (was 0.2f)
      skillDurationAccumulator(0.0f),                                                                                        // Initialize skill duration tracker
      patrolTimer(0.0f), patrolPhase(0),                                                                                     // Khởi tạo biến tuần tra
      hitCount(0),                                                                                                           // Initialize hit counter
      hitCooldown(0.0f),                                                                                                     // Initialize hit cooldown
      isBlinking(false), blinkingAcum(0), blinkingTime(0.1f), blinkingAcumTotal(0), doBlink(false), markedForRemoval(false), // Initialize blinking variables
      blinkingAlpha(1.0f), fadingOut(true)                                                                                   // Initialize smooth blinking variables
{
    frameTime = 0.35f; // Slower frame animation (was 0.2f)
    frameAcumulator = 0.0f;
    currentFrame = 0;
    maxFrames = 4;

    LoadTextures();
    UpdateTexture();

    behavior = nullptr;
    BuildBehaviorTree();
}

void Boss::BuildBehaviorTree()
{
    auto canSeeMario = new ConditionNode([this]()
                                         { return CanSeeMario(); });
    auto isCloseToMario = new ConditionNode([this]()
                                            { return IsCloseToMario(); });
    auto isAttackReady = new ConditionNode([this]()
                                           {
                                               return attackCount < maxAttacks; // Chỉ cần chưa đạt max attacks, không cần timer
                                           });
    auto shouldUseSkill = new ConditionNode([this]()
                                            {
                                                return attackCount >= maxAttacks && IsSkillReady(); // Skill khi đã đạt max attacks
                                            });

    auto notInActionState = new ConditionNode([this]()
                                              {
        // Only block BT when actively using skill, allow all other cases
        return !(currentState == BossState::SKILL && isUsingSkill); });

    auto doSkill = new ActionNode([this]()
                                  { SetState(BossState::SKILL); });
    auto doAttack = new ActionNode([this]()
                                   { SetState(BossState::ATTACK); });
    auto doChase = new ActionNode([this]()
                                  { SetState(BossState::CHASE); });
    auto doPatrol = new ActionNode([this]()
                                   { SetState(BossState::PATROL); });

    auto skillSequence = new SequenceNode({notInActionState, isCloseToMario, shouldUseSkill, doSkill});
    auto attackSequence = new SequenceNode({notInActionState, isCloseToMario, isAttackReady, doAttack});
    auto chaseSequence = new SequenceNode({notInActionState, canSeeMario, doChase});
    auto patrolAction = new SequenceNode({notInActionState, doPatrol});

    auto rootSelector = new SelectorNode({skillSequence,  // Skill có ưu tiên cao (khi attackCount >= 3)
                                          attackSequence, // Attack có ưu tiên thứ 2 (khi attackCount < 3)
                                          chaseSequence,
                                          patrolAction});

    behavior = new BehaviorTree(rootSelector);
}

Boss::~Boss()
{
    CleanupProjectiles();
    if (behavior)
    {
        delete behavior;
    }
}

void Boss::Update()
{
    if (behavior)
    {
        behavior->Tick();
    }

    float dt = GameClock::GetInstance().FIXED_TIME_STEP;

    attackTimer -= dt;

    skillTimer -= dt;
    if (skillTimer < 0)
        skillTimer = 0;

    // Update hit cooldown
    hitCooldown -= dt;
    if (hitCooldown < 0)
        hitCooldown = 0;

    // Update blinking effect
    UpdateBlinking();

    if (currentState == BossState::SKILL && !skillFlyFrames.empty())
    {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime)
        {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
        }
    }

    float dist = GetDistanceToMario();
    bool canSee = CanSeeMario();
    bool isClose = IsCloseToMario();
    bool attackReady = (attackTimer <= 0 && attackCount < maxAttacks);
    bool skillReady = IsSkillReady();
    bool shouldSkill = (attackCount >= maxAttacks && skillReady);
    bool notInAction = !(currentState == BossState::SKILL && isUsingSkill);

    switch (currentState)
    {
    case BossState::PATROL:
        Patrol(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::CHASE:
        Chase(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::ATTACK:
        Attack(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::SKILL:
        UseSkill(dt);
        break;
    default:
        break;
    }

    if (currentState == BossState::PATROL)
    {
        if (pos.x < 20)
        {
            pos.x = 20;
            vel.x = 0;
            direction = DIRECTION_RIGHT;
        }
        if (pos.x > 780)
        {
            pos.x = 780;
            vel.x = 0;
            direction = DIRECTION_LEFT;
        }
        if (pos.y < 350)
        {
            pos.y = 350;
            vel.y = 0;
        }
        if (pos.y > 650)
        {
            pos.y = 650;
            vel.y = 0;
        }
    }

    frameAcumulator += dt;
    if (frameAcumulator >= frameTime)
    {
        frameAcumulator = 0;

        switch (currentState)
        {
        case BossState::CHASE:
            if (!chaseFrames.empty())
            {
                currentFrame = (currentFrame + 1) % chaseFrames.size();
            }
            break;
        case BossState::PATROL:
            if (!movingFrames.empty())
            {
                currentFrame = (currentFrame + 1) % movingFrames.size();
            }
            break;
        case BossState::ATTACK:
            if (!attackFrames.empty())
            {
                currentFrame = (currentFrame + 1) % attackFrames.size();
            }
            break;
        case BossState::SKILL:
            break;
        default:
            if (!movingFrames.empty())
            {
                currentFrame = (currentFrame + 1) % movingFrames.size();
            }
            break;
        }
    }

    UpdateTexture();
    UpdateProjectiles(); // Update projectiles every frame
}

void Boss::UpdateStateAndPhysic()
{
    // Boss physics - không sử dụng gravity như các enemy khác
    float dt = GameClock::GetInstance().FIXED_TIME_STEP;

    attackTimer -= dt;
    skillTimer -= dt;
    if (skillTimer < 0)
        skillTimer = 0;

    // Update hit cooldown
    hitCooldown -= dt;
    if (hitCooldown < 0)
        hitCooldown = 0;

    // Update blinking effect
    UpdateBlinking();

    if (currentState == BossState::SKILL && !skillFlyFrames.empty())
    {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime)
        {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
        }
    }

    // Behavior tree logic
    if (behavior)
    {
        behavior->Tick();
    }

    switch (currentState)
    {
    case BossState::PATROL:
        Patrol(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::CHASE:
        Chase(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::ATTACK:
        Attack(dt);
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        break;
    case BossState::SKILL:
        UseSkill(dt);
        break;
    default:
        break;
    }

    // Boundary constraints
    if (currentState == BossState::PATROL)
    {
        if (pos.x < 20)
        {
            pos.x = 20;
            vel.x = 0;
            direction = DIRECTION_RIGHT;
        }
        if (pos.x > 780)
        {
            pos.x = 780;
            vel.x = 0;
            direction = DIRECTION_LEFT;
        }
        if (pos.y < 350)
        {
            pos.y = 350;
            vel.y = 0;
        }
        if (pos.y > 650)
        {
            pos.y = 650;
            vel.y = 0;
        }
    }

    // Animation updates
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime)
    {
        frameAcumulator = 0;

        switch (currentState)
        {
        case BossState::CHASE:
            if (!chaseFrames.empty())
            {
                currentFrame = (currentFrame + 1) % chaseFrames.size();
            }
            break;
        case BossState::PATROL:
            if (!movingFrames.empty())
            {
                currentFrame = (currentFrame + 1) % movingFrames.size();
            }
            break;
        case BossState::ATTACK:
            if (!attackFrames.empty())
            {
                currentFrame = (currentFrame + 1) % attackFrames.size();
            }
            break;
        case BossState::SKILL:
            break;
        default:
            if (!movingFrames.empty())
            {
                currentFrame = (currentFrame + 1) % movingFrames.size();
            }
            break;
        }
    }

    UpdateTexture();
    UpdateProjectiles();
    UpdateCollisionProbes(); // Update collision probes after movement
}

float Boss::GetDistanceToMario() const
{
    if (!marioPos)
    {
        return 999999.0f;
    }
    // Calculate from Boss center instead of top-left corner
    float bossCenterX = pos.x + size.x / 2;
    float bossCenterY = pos.y + size.y / 2;

    float distance = sqrt((marioPos->x - bossCenterX) * (marioPos->x - bossCenterX) +
                          (marioPos->y - bossCenterY) * (marioPos->y - bossCenterY));

    return distance;
}

Vector2 Boss::GetDirectionToMario() const
{
    if (!marioPos)
        return {0, 0};

    // Calculate from Boss center instead of top-left corner
    float bossCenterX = pos.x + size.x / 2;
    float bossCenterY = pos.y + size.y / 2;

    float dx = marioPos->x - bossCenterX;
    float dy = marioPos->y - bossCenterY;
    float distance = GetDistanceToMario();

    if (distance > 0)
    {
        return {dx / distance, dy / distance};
    }
    return {0, 0};
}

Vector2 Boss::GetPredictedDirectionToMario() const
{
    if (!marioPos)
        return {0, 0};

    // Basic prediction: assume Mario continues moving in current direction
    // Projectile speed is 300, so calculate time to reach Mario
    float projectileSpeed = 300.0f;
    float distance = GetDistanceToMario();
    float timeToReach = distance / projectileSpeed;

    // For now, we'll use simple prediction - in a full implementation,
    // you'd need access to Mario's velocity
    // Predict Mario's position assuming he continues current movement
    Vector2 predictedMarioPos = *marioPos;

    // Calculate from Boss center instead of top-left corner
    float bossCenterX = pos.x + size.x / 2;
    float bossCenterY = pos.y + size.y / 2;

    // Calculate direction to predicted position
    float dx = predictedMarioPos.x - bossCenterX;
    float dy = predictedMarioPos.y - bossCenterY;
    float predictedDistance = sqrt(dx * dx + dy * dy);

    if (predictedDistance > 0)
    {
        return {dx / predictedDistance, dy / predictedDistance};
    }
    return GetDirectionToMario(); // Fallback to current direction
}

void Boss::LoadTextures()
{
    try
    {
        ResrcManager &resrc = ResrcManager::GetInstance();

        idleFrames.clear();
        movingFrames.clear();
        chaseFrames.clear();
        attackFrames.clear();
        skillFlyFrames.clear();

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

        if (!attackFrames.empty())
        {
            currentTexture = attackFrames[0];
        }
        else if (!movingFrames.empty())
        {
            currentTexture = movingFrames[0];
        }
        else if (!chaseFrames.empty())
        {
            currentTexture = chaseFrames[0];
        }
        else
        {
            currentTexture = nullptr;
        }
    }
    catch (...)
    {
        currentTexture = nullptr;
    }
}

void Boss::UpdateTexture()
{
    switch (currentState)
    {
    case BossState::PATROL:
        if (!movingFrames.empty())
        {
            if (currentFrame >= movingFrames.size())
            {
                currentFrame = 0;
            }
            currentTexture = movingFrames[currentFrame];
            sprite = currentTexture;

            if (vel.x > 0)
                direction = DIRECTION_RIGHT;
            else if (vel.x < 0)
                direction = DIRECTION_LEFT;
        }
        break;

    case BossState::CHASE:
        if (!chaseFrames.empty())
        {
            if (currentFrame >= chaseFrames.size())
            {
                currentFrame = 0;
            }
            currentTexture = chaseFrames[currentFrame];

            if (currentTexture == nullptr)
            {
                currentFrame = 0;
                currentTexture = chaseFrames[0];
            }

            sprite = currentTexture;

            if (marioPos)
            {
                if (marioPos->x > pos.x)
                    direction = DIRECTION_RIGHT;
                else
                    direction = DIRECTION_LEFT;
            }
        }
        else
        {
            if (!movingFrames.empty())
            {
                currentTexture = movingFrames[0];
                sprite = currentTexture;
            }
        }
        break;

    case BossState::ATTACK:
        if (!attackFrames.empty())
        {
            if (currentFrame >= attackFrames.size())
            {
                currentFrame = 0;
            }
            currentTexture = attackFrames[currentFrame];
            sprite = currentTexture;

            if (marioPos)
            {
                if (marioPos->x > pos.x)
                    direction = DIRECTION_RIGHT;
                else
                    direction = DIRECTION_LEFT;
            }
        }
        break;

    case BossState::SKILL:
        if (!skillFlyFrames.empty())
        {
            currentTexture = skillFlyFrames[skillCurrentFrame];
            sprite = currentTexture;

            if (marioPos)
            {
                if (marioPos->x > pos.x)
                    direction = DIRECTION_RIGHT;
                else
                    direction = DIRECTION_LEFT;
            }
        }
        break;

    default:
        if (!movingFrames.empty())
        {
            currentTexture = movingFrames[0];
            sprite = currentTexture;
        }
        break;
    }
}

void Boss::Draw()
{
    if (sprite)
    {
        Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
        Rectangle dest = {pos.x, pos.y, size.x, size.y};
        Vector2 origin = {0, 0};

        if (direction == DIRECTION_RIGHT)
        {
            source.width = -abs(source.width);
        }
        else
        {
            source.width = abs(source.width);
        }

        // Apply blinking alpha effect for smooth transparency
        Color drawColor = color;
        if (isBlinking)
        {
            drawColor.a = (unsigned char)(255 * blinkingAlpha);
        }

        DrawTexturePro(*sprite, source, dest, origin, angle, drawColor);
    }
    else
    {
        // Apply alpha to fallback rectangle as well
        Color rectColor = RED;
        if (isBlinking)
        {
            rectColor.a = (unsigned char)(255 * blinkingAlpha);
        }
        DrawRectangle(pos.x, pos.y, size.x, size.y, rectColor);

        // Text alpha
        Color textColor = WHITE;
        if (isBlinking)
        {
            textColor.a = (unsigned char)(255 * blinkingAlpha);
        }
        DrawText("BOSS", pos.x + 10, pos.y + 20, 12, textColor);
    }

    string stateText = BossStateToString(currentState);
    string frameInfo = "Frame: " + std::to_string(currentFrame);
    if (currentState == BossState::CHASE)
    {
        frameInfo += "/" + std::to_string(chaseFrames.size()) + " Chase";
    }
    else if (currentState == BossState::PATROL)
    {
        frameInfo += "/" + std::to_string(movingFrames.size()) + " Patrol";
    }

    float dist = GetDistanceToMario();
    string debugInfo = "Dist: " + std::to_string((int)dist) +
                       " | AtkCnt: " + std::to_string(attackCount) +
                       " | AtkTimer: " + std::to_string(attackTimer) +
                       " | SkillTimer: " + std::to_string(skillTimer);

    // Draw health bar at the top of boss with alpha effect
    Texture2D *healthBarTexture = GetCurrentHealthBarTexture();
    if (healthBarTexture)
    {
        Rectangle healthBarSource = {0, 0, (float)healthBarTexture->width, (float)healthBarTexture->height};
        // Position health bar centered at the top of boss
        float healthBarWidth = 80.0f;                             // Slightly wider health bar
        float healthBarHeight = 12.0f;                            // Thinner health bar
        float healthBarX = pos.x + (size.x - healthBarWidth) / 2; // Center horizontally
        float healthBarY = pos.y - 25;                            // Position above boss

        Rectangle healthBarDest = {healthBarX, healthBarY, healthBarWidth, healthBarHeight};
        Vector2 healthBarOrigin = {0, 0};

        // Apply alpha to health bar
        Color healthBarColor = WHITE;
        if (isBlinking)
        {
            healthBarColor.a = (unsigned char)(255 * blinkingAlpha);
        }

        DrawTexturePro(*healthBarTexture, healthBarSource, healthBarDest, healthBarOrigin, 0.0f, healthBarColor);
    }

    // Apply alpha to debug text as well
    Color textColor = YELLOW;
    Color textColor2 = GREEN;
    Color textColor3 = WHITE;
    if (isBlinking)
    {
        textColor.a = (unsigned char)(255 * blinkingAlpha);
        textColor2.a = (unsigned char)(255 * blinkingAlpha);
        textColor3.a = (unsigned char)(255 * blinkingAlpha);
    }

    DrawText(stateText.c_str(), pos.x, pos.y - 60, 12, textColor);
    DrawText(frameInfo.c_str(), pos.x, pos.y - 45, 10, textColor2);
    DrawText(debugInfo.c_str(), pos.x, pos.y - 40, 8, textColor3);

    // Draw projectiles
    DrawProjectiles();
}

void Boss::FireProjectile(Vector2 direction)
{
    FireType projectileType = FireType::SMALL;

    int currentAttackInCycle = attackCount % maxAttacks;

    switch (currentAttackInCycle)
    {
    case 0:
        projectileType = FireType::SMALL;
        break;
    case 1:
        projectileType = FireType::NORMAL;
        break;
    case 2:
        projectileType = FireType::BIG;
        break;
    default:
        projectileType = FireType::SMALL;
        break;
    }

    Vector2 projectilePos = {
        pos.x + size.x / 2 - 16,
        pos.y + size.y / 2 - 16};

    Vector2 accurateDirection = GetPredictedDirectionToMario();

    BossFireball *projectile = new BossFireball(projectilePos, accurateDirection, projectileType);
    projectiles.push_back(projectile);
}

void Boss::OnStateEnter(BossState newState)
{
    bool shouldResetFrame = true;

    if ((currentState == BossState::PATROL && newState == BossState::CHASE) ||
        (currentState == BossState::CHASE && newState == BossState::PATROL))
    {
        shouldResetFrame = false;
    }

    if (currentState == newState)
    {
        return;
    }

    switch (newState)
    {
    case BossState::PATROL:
        patrolTimer = 0.0f; // Reset timer tuần tra
        patrolPhase = 0;
        break;
    case BossState::CHASE:
        break;
    case BossState::ATTACK:
        vel = {0.0f, 0.0f};
        // DON'T increment attackCount here - let Attack() function handle it when actually attacking
        break;
    case BossState::SKILL:
        vel = {0.0f, 0.0f};
        isUsingSkill = true;
        skillCurrentFrame = 0;
        skillFrameAccumulator = 0.0f;
        skillDurationAccumulator = 0.0f; // Reset skill duration when skill starts
        shouldResetFrame = true;
        break;
    default:
        break;
    }

    if (shouldResetFrame)
    {
        // Không reset patrolTimer ở đây nữa
    }
    else
    {
        switch (newState)
        {
        case BossState::CHASE:
            if (!chaseFrames.empty() && currentFrame >= chaseFrames.size())
            {
                currentFrame = currentFrame % chaseFrames.size();
            }
            break;
        case BossState::PATROL:
            if (!movingFrames.empty() && currentFrame >= movingFrames.size())
            {
                currentFrame = currentFrame % movingFrames.size();
            }
            break;
        case BossState::ATTACK:
            if (!attackFrames.empty() && currentFrame >= attackFrames.size())
            {
                currentFrame = currentFrame % attackFrames.size();
            }
            break;
        }
    }
}

void Boss::SetState(BossState newState)
{
    if (currentState != newState)
    {
        OnStateEnter(newState);
        currentState = newState;
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
    
    // Smooth acceleration/deceleration for more natural movement
    const float acceleration = 80.0f; // How fast boss accelerates
    const float maxChaseSpeed = 50.0f;
    const float minChaseSpeed = 8.0f;
    
    if (directionToMario.x == 0 && directionToMario.y == 0)
    {
        // Gradually slow down when no direction
        vel.x *= 0.9f;
        vel.y *= 0.9f;
    }
    else
    {
        // Calculate target velocity
        Vector2 targetVel = {
            directionToMario.x * currentSpeed,
            directionToMario.y * currentSpeed
        };
        
        // Gradually accelerate towards target velocity for smoother movement
        vel.x += (targetVel.x - vel.x) * acceleration * dt;
        vel.y += (targetVel.y - vel.y) * acceleration * dt;
        
        // Clamp speed to reasonable limits
        float currentVelMagnitude = sqrt(vel.x * vel.x + vel.y * vel.y);
        if (currentVelMagnitude > maxChaseSpeed)
        {
            vel.x = (vel.x / currentVelMagnitude) * maxChaseSpeed;
            vel.y = (vel.y / currentVelMagnitude) * maxChaseSpeed;
        }
        else if (currentVelMagnitude < minChaseSpeed && currentVelMagnitude > 0)
        {
            vel.x = (vel.x / currentVelMagnitude) * minChaseSpeed;
            vel.y = (vel.y / currentVelMagnitude) * minChaseSpeed;
        }
    }

    // Update direction based on movement, not just target direction
    if (vel.x < -1.0f)
    {
        direction = DIRECTION_LEFT;
    }
    else if (vel.x > 1.0f)
    {
        direction = DIRECTION_RIGHT;
    }
}

void Boss::Attack(float dt)
{
    // Don't stop completely - maintain some movement for more natural feel
    Vector2 directionToMario = GetPredictedDirectionToMario();
    
    // Slow down movement during attack but don't stop completely
    const float attackMoveSpeed = moveSpeed * 0.3f; // Move at 30% of normal speed
    const float positioningSpeed = 15.0f; // Speed for subtle positioning adjustments
    
    // Subtle positioning - try to maintain optimal attack distance
    float distanceToMario = GetDistanceToMario();
    float optimalAttackDistance = attackRange * 0.7f; // Stay at 70% of max attack range
    
    if (distanceToMario > optimalAttackDistance + 20.0f)
    {
        // Too far - move closer slowly
        vel.x += directionToMario.x * positioningSpeed * dt;
        vel.y += directionToMario.y * positioningSpeed * dt;
    }
    else if (distanceToMario < optimalAttackDistance - 20.0f)
    {
        // Too close - back away slowly
        vel.x -= directionToMario.x * positioningSpeed * dt;
        vel.y -= directionToMario.y * positioningSpeed * dt;
    }
    else
    {
        // Good distance - maintain slight circular movement for dynamic feel
        Vector2 perpendicular = {-directionToMario.y, directionToMario.x}; // 90 degree rotation
        vel.x += perpendicular.x * attackMoveSpeed * 0.5f * dt;
        vel.y += perpendicular.y * attackMoveSpeed * 0.5f * dt;
    }
    
    // Apply friction to prevent excessive speed buildup
    vel.x *= 0.95f;
    vel.y *= 0.95f;
    
    // Clamp velocity to reasonable limits during attack
    float maxAttackSpeed = attackMoveSpeed * 1.5f;
    float currentSpeed = sqrt(vel.x * vel.x + vel.y * vel.y);
    if (currentSpeed > maxAttackSpeed)
    {
        vel.x = (vel.x / currentSpeed) * maxAttackSpeed;
        vel.y = (vel.y / currentSpeed) * maxAttackSpeed;
    }

    // Always face Mario when attacking
    if (directionToMario.x < -0.1f)
    {
        direction = DIRECTION_LEFT;
    }
    else if (directionToMario.x > 0.1f)
    {
        direction = DIRECTION_RIGHT;
    }

    if (attackTimer <= 0)
    {
        FireProjectile(directionToMario);
        attackTimer = attackCooldown;
        attackCount++;

        int currentAttackInCycle = (attackCount - 1) % maxAttacks + 1;
    }
}

void Boss::Patrol(float dt)
{
    patrolTimer += dt;

    if (patrolTimer >= 3.0f)
    {
        patrolTimer = 0.0f;
        patrolPhase = (patrolPhase + 1) % 4;
    }

    switch (patrolPhase)
    {
    case 0:
        vel.x = moveSpeed;
        vel.y = 0;
        direction = DIRECTION_RIGHT;
        break;
    case 1:
        vel.x = 0;
        vel.y = moveSpeed;
        break;
    case 2:
        vel.x = -moveSpeed;
        vel.y = 0;
        direction = DIRECTION_LEFT;
        break;
    case 3:
        vel.x = 0;
        vel.y = -moveSpeed;
        break;
    }
}

void Boss::UseSkill(float dt)
{
    // Use instance variable instead of static - allows real-time Mario tracking
    skillDurationAccumulator += dt;
    const float skillDuration = 2.0f;

    if (skillDurationAccumulator >= skillDuration)
    {
        // Reset all skill-related flags
        skillDurationAccumulator = 0.0f; // Reset the duration tracker
        skillFrameAccumulator = 0.0f;    // Reset animation frame tracker
        isUsingSkill = false;            // Critical: Must reset this flag!
        skillTimer = skillCooldown;
        attackCount = 0;
        attackTimer = 0.0f; // Set negative to ensure attackTimer <= 0 condition is met

        // Force BehaviorTree to re-evaluate immediately with current Mario position
        if (behavior)
        {
            behavior->Tick();
        }
    }
}

void Boss::UpdateProjectiles()
{
    for (auto it = projectiles.begin(); it != projectiles.end();)
    {
        BossFireball *projectile = *it;
        if (projectile->isOutOfDistance() || projectile->GetState() == OBJECT_STATE_DEAD)
        {
            delete projectile;
            it = projectiles.erase(it);
        }
        else
        {
            projectile->UpdateStateAndPhysic();
            ++it;
        }
    }
}

void Boss::DrawProjectiles()
{
    for (BossFireball *projectile : projectiles)
    {
        if (projectile && !projectile->isOutOfDistance())
        {
            projectile->Draw();
        }
    }
}

void Boss::CleanupProjectiles()
{
    for (BossFireball *projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();
}

// void Boss::OnHitByFireball() {
//     std::cout << "[DEBUG] OnHitByFireball called! Current hitCooldown: " << hitCooldown << std::endl;
//     std::cout << "[DEBUG] FORCING HIT - Ignoring cooldown for testing!" << std::endl;

//     // TEMPORARILY DISABLE COOLDOWN FOR TESTING
//     // if (hitCooldown > 0) {
//     //     std::cout << "Boss hit but still in cooldown: " << hitCooldown << std::endl;
//     //     return;
//     // }

//     std::cout << "Boss hit! HitCount before: " << hitCount;
//     hitCount++;
//     std::cout << " -> after: " << hitCount << std::endl;
//     hitCooldown = hitCooldownTime;
//     std::cout << "[DEBUG] Set hitCooldown to: " << hitCooldown << std::endl;

//     // Check which health bar frame should be displayed
//     int expectedFrame = 1;
//     if (hitCount >= 8) expectedFrame = 5;
//     else if (hitCount >= 6) expectedFrame = 4;
//     else if (hitCount >= 4) expectedFrame = 3;
//     else if (hitCount >= 2) expectedFrame = 2;
//     else expectedFrame = 1;

//     std::cout << "Expected health bar frame: " << expectedFrame << std::endl;

//     // Change Boss main sprite frame based on damage level
//     if (hitCount == 2) {
//         std::cout << "Boss damaged! Changing sprite frame after 2 hits" << std::endl;
//         // Force frame change to show damage
//         currentFrame = 1; // Change to damaged frame
//         UpdateTexture(); // Update the sprite immediately
//     }
//     else if (hitCount == 4) {
//         std::cout << "Boss heavily damaged! Changing sprite frame after 4 hits" << std::endl;
//         currentFrame = 2; // More damaged frame
//         UpdateTexture();
//     }
//     else if (hitCount == 6) {
//         std::cout << "Boss critically damaged! Changing sprite frame after 6 hits" << std::endl;
//         currentFrame = 3; // Critical damage frame
//         UpdateTexture();
//     }

//     if (hitCount >= maxHits) {
//         std::cout << "Boss should die now after " << maxHits << " hits!" << std::endl;
//         Object::SetState(OBJECT_STATE_DEAD);
//     }
// }

void Boss::OnHitByFireball()
{
    if (hitCooldown > 0)
        return;

    hitCount++;
    hitCooldown = hitCooldownTime;

    if (hitCount >= maxHits)
    {
        // Start blinking effect when boss is about to die
        StartBlinking(1.5f, 0.1f); // Blink for 1.5 seconds with 0.1s interval
    }
}

void Boss::StartBlinking(float duration, float interval)
{
    isBlinking = true;
    blinkingAcum = 0;
    blinkingTime = interval;
    blinkingAcumTotal = 0;
    doBlink = false;
    blinkingAlpha = 1.0f;
    fadingOut = true;
}

void Boss::UpdateBlinking()
{
    if (!isBlinking)
        return;

    float dt = GameClock::GetInstance().FIXED_TIME_STEP;
    blinkingAcum += dt;
    blinkingAcumTotal += dt;

    // Smooth sine wave blinking effect for more natural appearance
    const float blinkSpeed = 6.0f; // Frequency of blinking (higher = faster)
    blinkingAlpha = 0.3f + 0.7f * (0.5f + 0.5f * sin(blinkingAcum * blinkSpeed));
    // Alpha ranges from 0.3 to 1.0 for smooth but visible blinking

    if (blinkingAcumTotal >= 1.5f)
    { // Blink for 1.5 seconds then die
        StopBlinking();
        markedForRemoval = true;
        Object::SetState(OBJECT_STATE_DEAD);
    }
}

bool Boss::IsBlinking() const
{
    return isBlinking;
}

void Boss::StopBlinking()
{
    isBlinking = false;
    doBlink = false;
    blinkingAcum = 0;
    blinkingAcumTotal = 0;
    blinkingAlpha = 1.0f;
    fadingOut = true;
}

// Texture2D* Boss::GetCurrentHealthBarTexture() const {
//     try {
//         ResrcManager& resrc = ResrcManager::GetInstance();

//         int healthBarFrame;
//         if (hitCount >= 8) {
//             healthBarFrame = 5;  // Almost dead (8-9 hits)
//         } else if (hitCount >= 6) {
//             healthBarFrame = 4;  // Low health (6-7 hits)
//         } else if (hitCount >= 4) {
//             healthBarFrame = 3;  // Half health (4-5 hits)
//         } else if (hitCount >= 2) {
//             healthBarFrame = 2;  // Slightly damaged (2-3 hits)
//         } else {
//             healthBarFrame = 1;  // Full health (0-1 hits)
//         }

//         std::cout << "HitCount: " << hitCount << " -> HealthBarFrame: " << healthBarFrame << std::endl;

//         std::string healthBarName = "HealthBar" + std::to_string(healthBarFrame);
//         return &resrc.getTexture(healthBarName);

//     } catch (...) {
//         std::cout << "Failed to load health bar texture!" << std::endl;
//         return nullptr;
//     }
// }

Texture2D *Boss::GetCurrentHealthBarTexture() const
{
    try
    {
        ResrcManager &resrc = ResrcManager::GetInstance();

        int healthBarFrame = (hitCount / 2) + 1;
        if (healthBarFrame > 5)
            healthBarFrame = 5;

        std::string healthBarName = "HealthBar" + std::to_string(healthBarFrame);
        return &resrc.getTexture(healthBarName);
    }
    catch (...)
    {
        return nullptr;
    }
}
