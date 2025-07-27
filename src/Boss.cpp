#include "Boss.h"
#include <cmath>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

Boss::Boss(Vector2 startPos, Vector2* marioPosition)
    : Enemy(startPos, {64.0f, 64.0f}, {0.0f, 0.0f}, WHITE, 0.8f, 0, DIRECTION_RIGHT), 
      currentState(BossState::PATROL), marioPos(marioPosition),
      detectionRange(300.0f), chaseRange(200.0f), attackRange(100.0f), 
      moveSpeed(10.0f), chaseSpeedMultiplier(1.5f),
      attackCooldown(1.5f), attackTimer(0.0f),
      attackCount(0), maxAttacks(3), skillCooldown(5.0f), skillTimer(0.0f),
      isUsingSkill(false), skillCurrentFrame(0), skillFrameTime(0.2f), skillFrameAccumulator(0.0f) {
    
    // Các thiết lập riêng cho Boss
    frameTime = 0.2f; // Thời gian giữa các frame (200ms cho animation mượt hơn)
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
    
    auto skillSequence = new SequenceNode({isCloseToMario, isSkillReady, doSkill});
    
    auto attackSequence = new SequenceNode({isCloseToMario, isAttackReady, doAttack});
    
    auto chaseSequence = new SequenceNode({canSeeMario, doChase});
    
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
}

void Boss::Update() {

    if (behavior) {
        behavior->Tick();
    }

    float dt = GameClock::GetInstance().FIXED_TIME_STEP; 
    
    attackTimer -= dt;
    if (attackTimer < 0) attackTimer = 0;
    
    skillTimer -= dt;
    if (skillTimer < 0) skillTimer = 0;

    if (currentState == BossState::SKILL && !skillFlyFrames.empty()) {
        skillFrameAccumulator += dt;
        if (skillFrameAccumulator >= skillFrameTime) {
            skillFrameAccumulator = 0.0f;
            skillCurrentFrame = (skillCurrentFrame + 1) % skillFlyFrames.size();
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
    if (!marioPos) return 999999.0f; 
    return sqrt((marioPos->x - pos.x) * (marioPos->x - pos.x) + (marioPos->y - pos.y) * (marioPos->y - pos.y));
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
            currentTexture = attackFrames[0]; // Start with attack state texture
        } else if (!movingFrames.empty()) {
            currentTexture = movingFrames[0]; // Start with patrol/moving
        } else if (!chaseFrames.empty()) {
            currentTexture = chaseFrames[0]; // Fallback to chase
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
    //viên đạn
}

void Boss::Draw() {
    if (sprite) {
        // Flip texture based on direction
        Rectangle source = {0, 0, (float)sprite->width, (float)sprite->height};
        Rectangle dest = {pos.x, pos.y, size.x, size.y};
        Vector2 origin = {0, 0};
        
        if (direction == DIRECTION_RIGHT) {
            source.width = -abs(source.width); // đảm bảo width luôn âm khi quay phải
        } else {
            source.width = abs(source.width);  // đảm bảo width luôn dương khi quay trái
        }
        
        DrawTexturePro(*sprite, source, dest, origin, angle, color);
    } else {
        DrawRectangle(pos.x, pos.y, size.x, size.y, RED);
        DrawText("BOSS", pos.x + 10, pos.y + 20, 12, WHITE);
    }
    
    // Debug: Show boss state
    string stateText = BossStateToString(currentState);
    DrawText(stateText.c_str(), pos.x, pos.y - 20, 12, YELLOW);
    
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
            vel = {0.0f, 0.0f}; 
            attackCount++;
            maxFrames = attackFrames.empty() ? 1 : attackFrames.size();
            break;
        case BossState::SKILL:
            vel = {0.0f, 0.0f}; 
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
    
    static float patrolTimer = 0.0f;
    static int patrolPhase = 0; 
    
    patrolTimer += dt;
    
    if (patrolTimer >= 3.0f) {
        patrolTimer = 0.0f;
        patrolPhase = (patrolPhase + 1) % 4;
    }
    
    switch (patrolPhase) {
        case 0: // đi phải
            vel.x = moveSpeed;
            vel.y = 0;
            direction = DIRECTION_RIGHT;
            break;
        case 1: // đi xuống
            vel.x = 0;
            vel.y = moveSpeed;
            break;
        case 2: // đi trái
            vel.x = -moveSpeed;
            vel.y = 0;
            direction = DIRECTION_LEFT;
            break;
        case 3: // đi lên
            vel.x = 0;
            vel.y = -moveSpeed;
            break;
    }

    // Chuyển sang CHASE nếu thấy Mario
    if(CanSeeMario()){
        SetState(BossState::CHASE);
    }
}

void Boss::Chase(float dt){
    Vector2 directionToMario = GetDirectionToMario();
    float currentSpeed = moveSpeed * chaseSpeedMultiplier;
    if (directionToMario.x == 0 && directionToMario.y == 0) {
        // Nếu Mario không di chuyển, giữ nguyên tốc độ
        currentSpeed = moveSpeed;
    } else {
        // Giới hạn tốc độ tối thiểu: 5 pixels/sec
        currentSpeed = max(currentSpeed, 5.0f);
    }
    currentSpeed = min(currentSpeed, 30.0f); // Max 30 pixels/sec
    
    // Di chuyển về phía Mario
    vel.x = directionToMario.x * currentSpeed;
    vel.y = directionToMario.y * currentSpeed;
    
    // Update direction for sprite flipping
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else if (directionToMario.x > 0) {
        direction = DIRECTION_RIGHT;
    }
    
    float distanceToMario = GetDistanceToMario();
    
    if (distanceToMario < attackRange) {
        SetState(BossState::ATTACK);
    }

    else if (distanceToMario > chaseRange) {
        SetState(BossState::PATROL);
    }
}

void Boss::Attack(float dt){
    vel.x = 0;
    vel.y = 0;
    
    Vector2 directionToMario = GetDirectionToMario();
    if (directionToMario.x < 0) {
        direction = DIRECTION_LEFT;
    } else {
        direction = DIRECTION_RIGHT;
    }
    
    if (attackTimer <= 0) {
        // Fire projectile towards Mario
        FireProjectile(directionToMario);
        
        // Reset attack timer
        attackTimer = attackCooldown;
        attackCount++;
        
        // Skill after multiple attacks
        if (attackCount >= maxAttacks && IsSkillReady()) {
            attackCount = 0;
            SetState(BossState::SKILL);
            return;
        }
    }
    
    float distanceToMario = GetDistanceToMario();
    
    if (distanceToMario > attackRange) {
        SetState(BossState::CHASE);
    }
    else if (distanceToMario > chaseRange) {
        SetState(BossState::PATROL);
    }
}

void Boss::UseSkill(float dt) {
    //skill nhảy hay gì đó
}