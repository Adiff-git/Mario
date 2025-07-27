#include "Boss.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Boss::Boss(Vector2 startPos, Vector2* marioPosition)
    : Enemy(startPos, {64.0f, 64.0f}, {0.0f, 0.0f}, WHITE, 0.8f, 0, DIRECTION_RIGHT), 
      currentState(BossState::PATROL), marioPos(marioPosition),
      detectionRange(300.0f), chaseRange(200.0f), attackRange(100.0f), 
      moveSpeed(20.0f), chaseSpeedMultiplier(1.5f),
      attackCooldown(1.5f), attackTimer(0.0f),
      attackCount(0), maxAttacks(3), skillCooldown(5.0f), skillTimer(0.0f),
      isUsingSkill(false), skillCurrentFrame(0), skillFrameTime(0.5f), skillFrameAccumulator(0.0f) {
    
    // Các thiết lập riêng cho Boss
    frameTime = 0.4f; //thời gian giữa các frame (làm chậm hơn nữa)
    frameAcumulator = 0.0f;// đếm
    currentFrame = 0;
    maxFrames = 4; // sẽ được update trong OnStateEnter 
    
    LoadTextures();
    UpdateTexture();
    
    behavior = nullptr; 
    BuildBehaviorTree(); 
}

void Boss::BuildBehaviorTree() {
    //khởi tạo
    auto canSeeMario = new ConditionNode([this]() { return CanSeeMario(); });
    auto isCloseToMario = new ConditionNode([this]() { return IsCloseToMario(); });
    auto isAttackReady = new ConditionNode([this]() { return attackTimer <= 0; });
    auto isSkillReady = new ConditionNode([this]() { return IsSkillReady(); });
    
    // Tạo các action nodes  
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
    
    // Tạo sequence cho skill: Gần Mario + Skill ready -> Skill
    auto skillSequence = new SequenceNode({isCloseToMario, isSkillReady, doSkill});
    
    // Tạo sequence cho attack: Gần Mario + Attack ready -> Attack
    auto attackSequence = new SequenceNode({isCloseToMario, isAttackReady, doAttack});
    
    // Tạo sequence cho chase: Thấy Mario -> Chase  
    auto chaseSequence = new SequenceNode({canSeeMario, doChase});
    
    //Skill -> Attack -> Chase -> Patrol (bỏ IDLE)
    auto rootSelector = new SelectorNode({
        skillSequence,
        attackSequence,  
        chaseSequence,  
        doPatrol        
    });
    
    behavior = new BehaviorTree(rootSelector);
}

Boss::~Boss() {
    if (behavior) {
        delete behavior;
    }
    // ResrcManager sẽ tự quản lý texture, không cần unload
}

void Boss::Update() {
    // Bật behavior tree để Boss hoạt động
    if (behavior) {
        behavior->Tick();
    }

    float dt = GetFrameTime(); 
    
    // Update timers
    attackTimer -= dt;
    if (attackTimer < 0) attackTimer = 0;
    
    skillTimer -= dt;
    if (skillTimer < 0) skillTimer = 0;

    // Update skill frame animation cho SKILL state
    if (currentState == BossState::SKILL && !skillFlyFrames.empty()) {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime) {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
        }
    }

    // Boss FSM - bỏ IDLE state
    switch (currentState) {
        case BossState::PATROL: 
            Patrol(dt); 
            // Update vị trí cho patrol
            pos.x += vel.x * dt;
            pos.y += vel.y * dt;
            break;
        case BossState::CHASE:  
            Chase(dt);  
            // Update vị trí cho chase
            pos.x += vel.x * dt;
            pos.y += vel.y * dt;
            break;
        case BossState::ATTACK: 
            Attack(dt); 
            // Attack có thể di chuyển một chút
            pos.x += vel.x * dt;
            pos.y += vel.y * dt;
            break;
        case BossState::SKILL: 
            UseSkill(dt); 
            // Skill thường đứng yên
            break;
        default: break;
    }

    // Giới hạn Boss trong màn hình
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

    // Update animation frame
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime && maxFrames > 0) {
        frameAcumulator = 0;
        currentFrame = (currentFrame + 1) % maxFrames;
    }
    
    UpdateTexture();
}

void Boss::UpdateStateAndPhysic() {
    // Call the existing Update method
    Update();
}

float Boss::GetDistanceToMario() const{
    if (!marioPos) return 999999.0f; // Fallback nếu marioPos null
    
    float dx = marioPos->x - pos.x;
    float dy = marioPos->y - pos.y;
    return sqrt(dx * dx + dy * dy);
}

Vector2 Boss::GetDirectionToMario() const {
    if (!marioPos) return {0, 0}; // Fallback nếu marioPos null
    
    float dx = marioPos->x - pos.x;
    float dy = marioPos->y - pos.y;
    float distance = sqrt(dx * dx + dy * dy);
    
    if (distance > 0) {
        return {dx / distance, dy / distance};
    }
    return {0, 0};
}

void Boss::LoadTextures() {
    try {
        ResrcManager& resrc = ResrcManager::GetInstance();
        
        // Clear tất cả các vector frames
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
        
        attackFrames.push_back(&resrc.getTexture("Skill 2_1"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_2"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_3"));
        attackFrames.push_back(&resrc.getTexture("Skill 2_4"));
        
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_1"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_2"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_3,6"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_4"));
        skillFlyFrames.push_back(&resrc.getTexture("Skill 3_5"));
    
        
        if (!idleFrames.empty()) {
            currentTexture = idleFrames[0]; // Start with idle
        } else {
            currentTexture = nullptr;
        }
    } catch (...) {
        // Fallback nếu không load được texture
        currentTexture = nullptr;
    }
}

void Boss::UpdateTexture() {
    switch (currentState) {
        case BossState::PATROL:
            if (!movingFrames.empty()) {
                maxFrames = movingFrames.size();
                int frameIndex = currentFrame % movingFrames.size();
                currentTexture = movingFrames[frameIndex];
                sprite = currentTexture;
            }
            break;
        case BossState::CHASE:
            if (!chaseFrames.empty()) {
                maxFrames = chaseFrames.size();
                int frameIndex = currentFrame % chaseFrames.size();
                currentTexture = chaseFrames[frameIndex];
                sprite = currentTexture;
            }
            break;
        case BossState::ATTACK:
            if (!attackFrames.empty()) {
                maxFrames = attackFrames.size();
                int frameIndex = currentFrame % attackFrames.size();
                currentTexture = attackFrames[frameIndex];
                sprite = currentTexture;
            }
            break;
        case BossState::SKILL:
            if (!skillFlyFrames.empty()) {
                maxFrames = skillFlyFrames.size();
                currentTexture = skillFlyFrames[skillCurrentFrame];
                sprite = currentTexture;
            }
            break;
        default:
            if (!movingFrames.empty()) {
                maxFrames = movingFrames.size();
                currentTexture = movingFrames[0];
                sprite = currentTexture;
            }
            break;
    }
}

void Boss::FireProjectile(Vector2 direction) {
    
}

void Boss::Draw() {
    if (sprite) {
        // Flip texture based on direction
        Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
        Rectangle dest = {pos.x, pos.y, size.x, size.y};
        Vector2 origin = {0, 0};
        
        if (direction == DIRECTION_LEFT) {
            source.width = -source.width; // Flip horizontally
        }
        
        DrawTexturePro(*sprite, source, dest, origin, angle, color);
    } else {
        // Fallback: draw a colored rectangle if no sprite
        DrawRectangle(pos.x, pos.y, size.x, size.y, RED);
        DrawText("BOSS", pos.x + 10, pos.y + 20, 12, WHITE);
    }
    
    // Debug: Show boss state
    string stateText = BossStateToString(currentState);
    DrawText(stateText.c_str(), pos.x, pos.y - 20, 12, YELLOW);
    
    // Debug: Draw detection range circle
    #ifdef DEBUG
    DrawCircleLines(pos.x + size.x/2, pos.y + size.y/2, detectionRange, RED);
    DrawCircleLines(pos.x + size.x/2, pos.y + size.y/2, chaseRange, ORANGE);
    DrawCircleLines(pos.x + size.x/2, pos.y + size.y/2, attackRange, YELLOW);
    #endif
}

void Boss::OnStateEnter(BossState newState) {
    currentFrame = 0;
    frameAcumulator = 0.0f;
    
    switch (newState) {
        case BossState::PATROL:
            maxFrames = movingFrames.empty() ? 1 : movingFrames.size();
            break;
        case BossState::CHASE:
            maxFrames = chaseFrames.empty() ? 1 : chaseFrames.size();
            break;
        case BossState::ATTACK:
            vel = {0.0f, 0.0f}; // Stop moving during attack
            attackCount++;
            maxFrames = attackFrames.empty() ? 1 : attackFrames.size();
            break;
        case BossState::SKILL:
            vel = {0.0f, 0.0f}; // Stop moving during skill
            isUsingSkill = true;
            skillCurrentFrame = 0; // Reset skill animatiIon
            skillFrameAccumulator = 0.0f;
            maxFrames = skillFlyFrames.empty() ? 1 : skillFlyFrames.size();
            break;
        default:
            maxFrames = 1;
            break;
    }
}

void Boss::SetState(BossState newState){
    if (currentState != newState) {
        // Debug: In ra khi chuyển state
        printf("Boss state change: %s -> %s\n", 
               BossStateToString(currentState).c_str(), 
               BossStateToString(newState).c_str());
        
        OnStateEnter(newState);
        currentState = newState;
    }
}

bool Boss::CanSeeMario() const{
    float dist = GetDistanceToMario();
    return dist < detectionRange; 
}

bool Boss::IsCloseToMario() const{
    float dist = GetDistanceToMario();
    return dist < attackRange;
}

void Boss::Patrol(float dt){
    vel.x = moveSpeed * (direction == DIRECTION_RIGHT ? 1 : -1);
    vel.y = 0.0f; 
    
    // Giới hạn Boss trong khu vực an toàn
    if (pos.x < 200) {
        direction = DIRECTION_RIGHT;
        pos.x = 200; // Không cho đi quá trái
    }
    if (pos.x > 600) {
        direction = DIRECTION_LEFT;
        pos.x = 600; // Không cho đi quá phải
    }

    if(CanSeeMario()){
        SetState(BossState::CHASE);
    } else if (IsCloseToMario()) {
        SetState(BossState::ATTACK);
    }
    // Bỏ transition về IDLE - chỉ patrol
}

void Boss::Chase(float dt){
    Vector2 directionToMario = GetDirectionToMario();
    float currentSpeed = moveSpeed * chaseSpeedMultiplier;
    
    // Giới hạn tốc độ chase để không quá nhanh
    currentSpeed = fmin(currentSpeed, 80.0f); // Max 80 pixels/sec
    
    // Di chuyển về phía Mario với tốc độ được giới hạn
    vel.x = directionToMario.x * currentSpeed;
    vel.y = directionToMario.y * currentSpeed;
    
    // Update direction for sprite flipping
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else if (directionToMario.x > 0) {
        direction = DIRECTION_RIGHT;
    }
    
    float distanceToMario = GetDistanceToMario();
    
    // Transition conditions
    if (distanceToMario < attackRange && attackTimer <= 0) {
        SetState(BossState::ATTACK);
    } else if (distanceToMario > chaseRange) {
        SetState(BossState::PATROL);
    } else if (distanceToMario > detectionRange) {
        SetState(BossState::PATROL); // Chuyển về PATROL thay vì IDLE
    }
}

void Boss::Attack(float dt){
    if (attackTimer > 0) return; 

    // Face Mario before attacking
    Vector2 directionToMario = GetDirectionToMario();
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else {
        direction = DIRECTION_RIGHT;
    }
    
    // Simple attack: Fire projectile towards Mario
    FireProjectile(directionToMario);
    
    // Reset attack timer
    attackTimer = attackCooldown;
    
    // Check if should use skill after multiple attacks
    if (attackCount >= maxAttacks && IsSkillReady()) {
        attackCount = 0; // Reset attack count
        SetState(BossState::SKILL);
        return;
    }
    
    // Transition back to chase if Mario is still in range
    float distanceToMario = GetDistanceToMario();
    if (distanceToMario > attackRange) {
        if (distanceToMario < chaseRange) {
            SetState(BossState::CHASE);
        } else {
            SetState(BossState::PATROL);
        }
    }
}

void Boss::UseSkill(float dt) {
    static float skillDuration = 2.0f; // Thời gian thực hiện skill
    static float skillTimer = 0.0f;
    
    skillTimer += dt;
    
    // Face Mario during skill
    Vector2 directionToMario = GetDirectionToMario();
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else {
        direction = DIRECTION_RIGHT;
    }
    
    // Fire multiple projectiles during skill
    static float fireRate = 0.3f; // Bắn mỗi 0.3 giây
    static float fireTimer = 0.0f;
    
    fireTimer += dt;
    if (fireTimer >= fireRate) {
        fireTimer = 0.0f;
        
        // Fire projectiles in multiple directions
        float angleStep = 45.0f; // 45 degrees between each shot
        for (int i = -1; i <= 1; i++) {
            float angle = atan2(directionToMario.y, directionToMario.x) + (i * angleStep * PI / 180.0f);
            Vector2 fireDirection = {cos(angle), sin(angle)};
            FireProjectile(fireDirection);
        }
    }
    
    // End skill after duration
    if (skillTimer >= skillDuration) {
        skillTimer = 0.0f;
        isUsingSkill = false;
        this->skillTimer = skillCooldown; // Set cooldown
        
        // Return to appropriate state
        float distanceToMario = GetDistanceToMario();
        if (distanceToMario < attackRange) {
            SetState(BossState::ATTACK);
        } else if (distanceToMario < chaseRange) {
            SetState(BossState::CHASE);
        } else {
            SetState(BossState::PATROL);
        }
    }
}