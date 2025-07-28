#include "Boss.h"
#include <cmath>
#include <iostream>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Boss::Boss(Vector2 startPos, Vector2* marioPosition)
    : Enemy(startPos, {64.0f, 64.0f}, {0.0f, 0.0f}, WHITE, 0.8f, 0, DIRECTION_RIGHT),
      currentState(BossState::PATROL), marioPos(marioPosition),
      detectionRange(300.0f), chaseRange(200.0f), attackRange(100.0f),
      moveSpeed(10.0f), chaseSpeedMultiplier(1.5f),
      attackCooldown(0.5f), attackTimer(0.0f),  // Giảm từ 1.5f xuống 0.5f
      attackCount(0), maxAttacks(3), skillCooldown(5.0f), skillTimer(0.0f),
      isUsingSkill(false), skillCurrentFrame(0), skillFrameTime(0.2f), skillFrameAccumulator(0.0f),
      skillDurationAccumulator(0.0f), // Initialize skill duration tracker
      patrolTimer(0.0f), patrolPhase(0), // Khởi tạo biến tuần tra
      hitCount(0), // Initialize hit counter
      hitCooldown(0.0f) // Initialize hit cooldown
{
    frameTime = 0.2f;
    frameAcumulator = 0.0f;
    currentFrame = 0;
    maxFrames = 4;

    LoadTextures();
    UpdateTexture();

    behavior = nullptr;
    BuildBehaviorTree();
}

void Boss::BuildBehaviorTree() {
    auto canSeeMario = new ConditionNode([this]() {
        return CanSeeMario();
    });
    auto isCloseToMario = new ConditionNode([this]() {
        return IsCloseToMario();
    });
    auto isAttackReady = new ConditionNode([this]() {
        return attackCount < maxAttacks;  // Chỉ cần chưa đạt max attacks, không cần timer
    });
    auto shouldUseSkill = new ConditionNode([this]() {
        return attackCount >= maxAttacks && IsSkillReady();  // Skill khi đã đạt max attacks
    });

    auto notInActionState = new ConditionNode([this]() {
        // Only block BT when actively using skill, allow all other cases
        return !(currentState == BossState::SKILL && isUsingSkill);
    });

    auto doSkill = new ActionNode([this]() {
        SetState(BossState::SKILL);
    });
    auto doAttack = new ActionNode([this]() {
        SetState(BossState::ATTACK);
    });
    auto doChase = new ActionNode([this]() {
        SetState(BossState::CHASE);
    });
    auto doPatrol = new ActionNode([this]() {
        SetState(BossState::PATROL);
    });

    auto skillSequence = new SequenceNode({notInActionState, isCloseToMario, shouldUseSkill, doSkill});
    auto attackSequence = new SequenceNode({notInActionState, isCloseToMario, isAttackReady, doAttack});
    auto chaseSequence = new SequenceNode({notInActionState, canSeeMario, doChase});
    auto patrolAction = new SequenceNode({notInActionState, doPatrol});

    auto rootSelector = new SelectorNode({
        skillSequence,      // Skill có ưu tiên cao (khi attackCount >= 3)
        attackSequence,     // Attack có ưu tiên thứ 2 (khi attackCount < 3)
        chaseSequence,
        patrolAction
    });

    behavior = new BehaviorTree(rootSelector);
}

Boss::~Boss() {
    CleanupProjectiles();
    if (behavior) {
        delete behavior;
    }
}

void Boss::Update() {
    if (behavior) {
        behavior->Tick();
    }

    float dt = GameClock::GetInstance().FIXED_TIME_STEP;

    attackTimer -= dt;
    // Allow attackTimer to go negative for proper condition checking
    // if (attackTimer < 0) attackTimer = 0;

    skillTimer -= dt;
    if (skillTimer < 0) skillTimer = 0;

    // Update hit cooldown
    hitCooldown -= dt;
    if (hitCooldown < 0) hitCooldown = 0;

    if (currentState == BossState::SKILL && !skillFlyFrames.empty()) {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime) {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
        }
    }

    // Debug output for conditions (less spam)
    
    float dist = GetDistanceToMario();
    bool canSee = CanSeeMario();
    bool isClose = IsCloseToMario();
    bool attackReady = (attackTimer <= 0 && attackCount < maxAttacks);
    bool skillReady = IsSkillReady();
    bool shouldSkill = (attackCount >= maxAttacks && skillReady);
    bool notInAction = !(currentState == BossState::SKILL && isUsingSkill);
    
    // Debug positions - kiểm tra vị trí thực tế (only when close or state change)
    static int debugCounter = 0;
    debugCounter++;
    if (debugCounter % 60 == 0 || isClose || currentState == BossState::ATTACK) { // Print every 1 second or when close
        if (marioPos) {
            std::cout << "Boss: (" << (int)pos.x << "," << (int)pos.y << ") Mario: (" 
                      << (int)marioPos->x << "," << (int)marioPos->y << ")" << std::endl;
            std::cout << "Dist: " << (int)dist << " | Close: " << isClose 
                      << " | AttackReady: " << attackReady << " | Count: " << attackCount 
                      << " | Timer: " << attackTimer << " | State: " << (int)currentState << std::endl;
        } else {
            std::cout << "ERROR: marioPos is NULL!" << std::endl;
        }
    }

    switch (currentState) {
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
        default: break;
    }

    if (currentState == BossState::PATROL) {
        if (pos.x < 50) {
            pos.x = 50;
            vel.x = 0;
            direction = DIRECTION_RIGHT;
        }
        if (pos.x > 750) {
            pos.x = 750;
            vel.x = 0;
            direction = DIRECTION_LEFT;
        }
        if (pos.y < 400) {
            pos.y = 400;
            vel.y = 0;
        }
        if (pos.y > 600) {
            pos.y = 600;
            vel.y = 0;
        }
    }

    frameAcumulator += dt;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0;

        switch (currentState) {
            case BossState::CHASE:
                if (!chaseFrames.empty()) {
                    currentFrame = (currentFrame + 1) % chaseFrames.size();
                }
                break;
            case BossState::PATROL:
                if (!movingFrames.empty()) {
                    currentFrame = (currentFrame + 1) % movingFrames.size();
                }
                break;
            case BossState::ATTACK:
                if (!attackFrames.empty()) {
                    currentFrame = (currentFrame + 1) % attackFrames.size();
                }
                break;
            case BossState::SKILL:
                break;
            default:
                if (!movingFrames.empty()) {
                    currentFrame = (currentFrame + 1) % movingFrames.size();
                }
                break;
        }
    }

    UpdateTexture();
    UpdateProjectiles();  // Update projectiles every frame
}

void Boss::UpdateStateAndPhysic() {
    Update();
}

float Boss::GetDistanceToMario() const {
    if (!marioPos) {
        return 999999.0f;
    }
    float distance = sqrt((marioPos->x - pos.x) * (marioPos->x - pos.x) + (marioPos->y - pos.y) * (marioPos->y - pos.y));

    return distance;
}

Vector2 Boss::GetDirectionToMario() const {
    if (!marioPos) return {0, 0};

    float dx = marioPos->x - pos.x;
    float dy = marioPos->y - pos.y;
    float distance = GetDistanceToMario();

    if (distance > 0) {
        return {dx / distance, dy / distance};
    }
    return {0, 0};
}

Vector2 Boss::GetPredictedDirectionToMario() const {
    if (!marioPos) return {0, 0};

    // Basic prediction: assume Mario continues moving in current direction
    // Projectile speed is 300, so calculate time to reach Mario
    float projectileSpeed = 300.0f;
    float distance = GetDistanceToMario();
    float timeToReach = distance / projectileSpeed;
    
    // For now, we'll use simple prediction - in a full implementation,
    // you'd need access to Mario's velocity
    // Predict Mario's position assuming he continues current movement
    Vector2 predictedMarioPos = *marioPos;
    
    // Calculate direction to predicted position
    float dx = predictedMarioPos.x - pos.x;
    float dy = predictedMarioPos.y - pos.y;
    float predictedDistance = sqrt(dx * dx + dy * dy);

    if (predictedDistance > 0) {
        return {dx / predictedDistance, dy / predictedDistance};
    }
    return GetDirectionToMario(); // Fallback to current direction
}

void Boss::LoadTextures() {
    try {
        ResrcManager& resrc = ResrcManager::GetInstance();
        
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
    
        if (!attackFrames.empty()) {
            currentTexture = attackFrames[0];
        } else if (!movingFrames.empty()) {
            currentTexture = movingFrames[0];
        } else if (!chaseFrames.empty()) {
            currentTexture = chaseFrames[0];
        } else {
            currentTexture = nullptr;
        }
    } catch (...) {
        currentTexture = nullptr;
    }
}

void Boss::UpdateTexture() {
    switch (currentState) {
        case BossState::PATROL:
            if (!movingFrames.empty()) {
                if (currentFrame >= movingFrames.size()) {
                    currentFrame = 0;
                }
                currentTexture = movingFrames[currentFrame];
                sprite = currentTexture;

                if (vel.x > 0) direction = DIRECTION_RIGHT;
                else if (vel.x < 0) direction = DIRECTION_LEFT;
            }
            break;

        case BossState::CHASE:
            if (!chaseFrames.empty()) {
                if (currentFrame >= chaseFrames.size()) {
                    currentFrame = 0;
                }
                currentTexture = chaseFrames[currentFrame];
                
                if (currentTexture == nullptr) {
                    currentFrame = 0;
                    currentTexture = chaseFrames[0];
                }
                
                sprite = currentTexture;

                if (marioPos) {
                    if (marioPos->x > pos.x) direction = DIRECTION_RIGHT;
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
                if (currentFrame >= attackFrames.size()) {
                    currentFrame = 0;
                }
                currentTexture = attackFrames[currentFrame];
                sprite = currentTexture;

                if (marioPos) {
                    if (marioPos->x > pos.x) direction = DIRECTION_RIGHT;
                    else direction = DIRECTION_LEFT;
                }
            }
            break;

        case BossState::SKILL:
            if (!skillFlyFrames.empty()) {
                currentTexture = skillFlyFrames[skillCurrentFrame];
                sprite = currentTexture;

                if (marioPos) {
                    if (marioPos->x > pos.x) direction = DIRECTION_RIGHT;
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

void Boss::Draw() {
    if (sprite) {
        Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
        Rectangle dest = {pos.x, pos.y, size.x, size.y};
        Vector2 origin = {0, 0};
        
        if (direction == DIRECTION_RIGHT) {
            source.width = -abs(source.width); 
        } else {
            source.width = abs(source.width);  
        }
        
        DrawTexturePro(*sprite, source, dest, origin, angle, color);
    } else {
        DrawRectangle(pos.x, pos.y, size.x, size.y, RED);
        DrawText("BOSS", pos.x + 10, pos.y + 20, 12, WHITE);
    }
    
    string stateText = BossStateToString(currentState);
    string frameInfo = "Frame: " + std::to_string(currentFrame);
    if (currentState == BossState::CHASE) {
        frameInfo += "/" + std::to_string(chaseFrames.size()) + " Chase";
    } else if (currentState == BossState::PATROL) {
        frameInfo += "/" + std::to_string(movingFrames.size()) + " Patrol";
    }
    
    // Debug info for attack state
    float dist = GetDistanceToMario();
    string debugInfo = "Dist: " + std::to_string((int)dist) + 
                       " | AtkCnt: " + std::to_string(attackCount) + 
                       " | AtkTimer: " + std::to_string(attackTimer) +
                       " | SkillTimer: " + std::to_string(skillTimer);
    
    string hitInfo = "HP: " + std::to_string(hitCount) + "/" + std::to_string(maxHits) + 
                     " CD: " + std::to_string((int)(hitCooldown * 10.0f)) + "/10";
    
    DrawText(stateText.c_str(), pos.x, pos.y - 60, 12, YELLOW);
    DrawText(frameInfo.c_str(), pos.x, pos.y - 45, 10, GREEN);
    DrawText(debugInfo.c_str(), pos.x, pos.y - 30, 8, WHITE);
    DrawText(hitInfo.c_str(), pos.x, pos.y - 15, 10, RED);
    
    // Draw projectiles
    DrawProjectiles();
}

void Boss::FireProjectile(Vector2 direction) {
    // Progressive power increase within each attack cycle (0,1,2 -> Small,Normal,Big)
    FireType projectileType = FireType::SMALL;
    
    // Use modulo to cycle through power levels in each attack sequence
    int currentAttackInCycle = attackCount % maxAttacks; // Will be 0, 1, or 2
    
    switch (currentAttackInCycle) {
        case 0:
            projectileType = FireType::SMALL;   // First attack in cycle
            break;
        case 1:
            projectileType = FireType::NORMAL;  // Second attack in cycle  
            break;
        case 2:
            projectileType = FireType::BIG;     // Third attack in cycle (before skill)
            break;
        default:
            projectileType = FireType::SMALL;   // Fallback
            break;
    }
    
    // Create projectile at boss center with slight offset toward Mario
    Vector2 projectilePos = {
        pos.x + size.x / 2 - 16,  // Center horizontally and offset for projectile size
        pos.y + size.y / 2 - 16   // Center vertically and offset for projectile size
    };
    
    // Get more accurate direction to Mario
    Vector2 accurateDirection = GetPredictedDirectionToMario();
    
    // Create and add projectile to list
    BossFireball* projectile = new BossFireball(projectilePos, accurateDirection, projectileType);
    projectiles.push_back(projectile);
    
    std::cout << "Boss fired projectile! Type: " << (int)projectileType 
              << " (Attack " << (currentAttackInCycle + 1) << "/3 in cycle, Total: " << attackCount << ")" 
              << " Direction: (" << accurateDirection.x << ", " << accurateDirection.y << ")" << std::endl;
}

void Boss::OnStateEnter(BossState newState) {
    bool shouldResetFrame = true;

    if ((currentState == BossState::PATROL && newState == BossState::CHASE) ||
        (currentState == BossState::CHASE && newState == BossState::PATROL)) {
        shouldResetFrame = false;
    }

    if (currentState == newState) {
        return;
    }

    switch (newState) {
        case BossState::PATROL:
            patrolTimer = 0.0f;  // Reset timer tuần tra
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

    if (shouldResetFrame) {
        // Không reset patrolTimer ở đây nữa
    } else {
        switch (newState) {
            case BossState::CHASE:
                if (!chaseFrames.empty() && currentFrame >= chaseFrames.size()) {
                    currentFrame = currentFrame % chaseFrames.size();
                }
                break;
            case BossState::PATROL:
                if (!movingFrames.empty() && currentFrame >= movingFrames.size()) {
                    currentFrame = currentFrame % movingFrames.size();
                }
                break;
            case BossState::ATTACK:
                if (!attackFrames.empty() && currentFrame >= attackFrames.size()) {
                    currentFrame = currentFrame % attackFrames.size();
                }
                break;
        }
    }
}

void Boss::SetState(BossState newState) {
    if (currentState != newState) {
        OnStateEnter(newState);
        currentState = newState;
    }
}

bool Boss::CanSeeMario() const {
    float dist = GetDistanceToMario();
    bool canSee = dist < detectionRange;
    return canSee;
}

bool Boss::IsCloseToMario() const{
    float dist = GetDistanceToMario();
    bool isClose = dist < attackRange;
    return isClose;
}

void Boss::Chase(float dt){
    Vector2 directionToMario = GetDirectionToMario();
    float currentSpeed = moveSpeed * chaseSpeedMultiplier;
    if (directionToMario.x == 0 && directionToMario.y == 0) {
        currentSpeed = moveSpeed;
    } else {
        currentSpeed = max(currentSpeed, 5.0f);
    }
    currentSpeed = min(currentSpeed, 30.0f);
    
    vel.x = directionToMario.x * currentSpeed;
    vel.y = directionToMario.y * currentSpeed;
    
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else if (directionToMario.x > 0) {
        direction = DIRECTION_RIGHT;
    }
}

void Boss::Attack(float dt){
    vel.x = 0;
    vel.y = 0;
    
    Vector2 directionToMario = GetPredictedDirectionToMario();
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else {
        direction = DIRECTION_RIGHT;
    }
    
    if (attackTimer <= 0) {
        FireProjectile(directionToMario);
        attackTimer = attackCooldown;
        attackCount++;
        
        int currentAttackInCycle = (attackCount - 1) % maxAttacks + 1; // Display as 1, 2, 3
        std::cout << "Boss attacks! Attack " << currentAttackInCycle << "/3 in cycle. " 
                  << "Predicted direction: (" << directionToMario.x 
                  << ", " << directionToMario.y << ")" << std::endl;
        
        // DON'T manually change state - let BehaviorTree handle all transitions
        // BT will check shouldUseSkill condition next frame and decide automatically
    }
}

void Boss::Patrol(float dt) {
    patrolTimer += dt;

    if (patrolTimer >= 3.0f) {
        patrolTimer = 0.0f;
        patrolPhase = (patrolPhase + 1) % 4;
    }

    switch (patrolPhase) {
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

void Boss::UseSkill(float dt) {
    // Use instance variable instead of static - allows real-time Mario tracking
    skillDurationAccumulator += dt;
    const float skillDuration = 2.0f;

    if (skillDurationAccumulator >= skillDuration) {
        // Reset all skill-related flags
        skillDurationAccumulator = 0.0f;  // Reset the duration tracker
        skillFrameAccumulator = 0.0f;     // Reset animation frame tracker
        isUsingSkill = false;             // Critical: Must reset this flag!
        skillTimer = skillCooldown;   
        attackCount = 0;
        attackTimer = 0.0f;              // Set negative to ensure attackTimer <= 0 condition is met

        // Force BehaviorTree to re-evaluate immediately with current Mario position
        if (behavior) {
            behavior->Tick();
        }
    }
}

void Boss::UpdateProjectiles() {
    for (auto it = projectiles.begin(); it != projectiles.end(); ) {
        BossFireball* projectile = *it;
        if (projectile->isOutOfDistance() || projectile->GetState() == OBJECT_STATE_DEAD) {
            std::cout << "Removing BossFireball - OutOfDistance: " << projectile->isOutOfDistance() 
                      << " Dead: " << (projectile->GetState() == OBJECT_STATE_DEAD) << std::endl;
            delete projectile;
            it = projectiles.erase(it);
        } else {
            projectile->UpdateStateAndPhysic();
            ++it;
        }
    }
}

void Boss::DrawProjectiles() {
    for (BossFireball* projectile : projectiles) {
        if (projectile && !projectile->isOutOfDistance()) {
            projectile->Draw();
        }
    }
}

void Boss::CleanupProjectiles() {
    for (BossFireball* projectile : projectiles) {
        delete projectile;
    }
    projectiles.clear();
}

void Boss::OnHitByFireball() {
    // Check if still in hit cooldown
    if (hitCooldown > 0) {
        std::cout << "Boss still in hit cooldown: " << hitCooldown << "s remaining" << std::endl;
        return; // Don't process hit if still in cooldown
    }
    
    hitCount++;
    hitCooldown = hitCooldownTime; // Set cooldown timer
    std::cout << "Boss hit by fireball! Hits: " << hitCount << "/" << maxHits << " (Cooldown set)" << std::endl;
    
    if (hitCount >= maxHits) {
        std::cout << "Boss defeated after " << maxHits << " hits!" << std::endl;
        Object::SetState(OBJECT_STATE_DEAD);  // Use Object::SetState to set object state
    }
}
