#include "Boss.h"
#include <cmath>

Boss::Boss(Vector2 startPos, Vector2* marioPosition)
    : Object(), currentState(BossState::IDLE), marioPos(marioPosition),
      detectionRange(300.0f), chaseRange(200.0f), attackRange(100.0f), moveSpeed(150.0f),
      attackCooldown(1.0f), attackTimer(0.0f) {
    pos = startPos;
    
    vel = {0.0f, 0.0f};
    size = {64.0f, 64.0f}; //size of the boss
    angle = 0.0f;
    color = WHITE;
    direction = DIRECTION_RIGHT;
    sprite = nullptr; // set sprite
    
    frameTime = 0.1f; //thời gian giữa các frame
    frameAcumulator = 0.0f;// đếm
    currentFrame = 0;
    maxFrames = 4; // chưa biết bao nhiêu 
    
    behavior = nullptr; 
    BuildBehaviorTree(); 
}

void Boss::BuildBehaviorTree() {
    //khởi tạo
    auto canSeeMario = new ConditionNode([this]() { return CanSeeMario(); });
    auto isCloseToMario = new ConditionNode([this]() { return IsCloseToMario(); });
    auto isAttackReady = new ConditionNode([this]() { return attackTimer <= 0; });
    
    // Tạo các action nodes  
    auto doAttack = new ActionNode([this]() { 
        SetState(BossState::ATTACK); 
    });
    auto doChase = new ActionNode([this]() { 
        SetState(BossState::CHASE); 
    });
    auto doPatrol = new ActionNode([this]() { 
        SetState(BossState::PATROL); 
    });
    auto doIdle = new ActionNode([this]() { 
        SetState(BossState::IDLE); 
    });
    
    // Tạo sequence cho attack: Gần Mario + Attack ready -> Attack
    auto attackSequence = new SequenceNode({isCloseToMario, isAttackReady, doAttack});
    
    // Tạo sequence cho chase: Thấy Mario -> Chase  
    auto chaseSequence = new SequenceNode({canSeeMario, doChase});
    
    //Attack -> Chase -> Patrol -> Idle
    auto rootSelector = new SelectorNode({
        attackSequence,  
        chaseSequence,  
        doPatrol,        
        doIdle           
    });
    \
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

    float dt = GetFrameTime(); 
    attackTimer -= dt;
    if (attackTimer < 0) attackTimer = 0;

    // FSM
    switch (currentState) {
        case BossState::IDLE: Idle(dt); break;
        case BossState::PATROL: Patrol(dt); break;
        case BossState::CHASE:  Chase(dt);  break;
        case BossState::ATTACK: Attack(dt); break;
        default: break;
    }

    // Update vị trí dựa trên velocity
    pos.x += vel.x * dt;
    pos.y += vel.y * dt;

    // Update animation frame
    frameAcumulator += dt;
    if (frameAcumulator >= frameTime) {
        frameAcumulator = 0;
        currentFrame = (currentFrame + 1) % maxFrames;
    }
}

float Boss::GetDistanceToMario() const{
    float dx = marioPos->x - pos.x;
    float dy = marioPos->y - pos.y;
    return sqrt(dx * dx + dy * dy);
}

void Boss::Draw() {
    //hàm draw này từ từ tính
    DrawTexture(*sprite, pos.x, pos.y, color);
}

void Boss::OnStateEnter(BossState newState) {
    // Reset animation when entering new state
    currentFrame = 0;
    frameAcumulator = 0.0f;
    
    // State-specific initialization
    switch (newState) {
        case BossState::IDLE:
            vel = {0.0f, 0.0f};
            break;
        case BossState::PATROL:
            // Initialize patrol behavior
            break;
        case BossState::CHASE:
            // Initialize chase behavior
            break;
        case BossState::ATTACK:
            // Initialize attack behavior
            vel = {0.0f, 0.0f}; // Stop moving during attack
            break;
        default:
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
    vel.x = moveSpeed * (direction == DIRECTION_RIGHT ? 1 : -1);
    vel.y = 0.0f; //chỗ này có thể sửa nếu muốn lên xuống

    if (pos.x < 100) direction = DIRECTION_RIGHT;
    if (pos.x > 800) direction = DIRECTION_LEFT;

    if(CanSeeMario()){
        SetState(BossState::CHASE);
    } else if (IsCloseToMario()) {
        SetState(BossState::ATTACK);
    } else {
        SetState(BossState::IDLE);
    }
}

void Boss::Chase(float dt){
    float marioX = marioPos->x;
    float marioY = marioPos->y;

    if (marioX < pos.x) {
        vel.x = -moveSpeed;
        direction = DIRECTION_LEFT;
    } else {
        vel.x = moveSpeed;
        direction = DIRECTION_RIGHT;
    }

    if (marioY < pos.y) {
        vel.y = -moveSpeed;
        direction = DIRECTION_UP;
    } else {
        vel.y = moveSpeed;
        direction = DIRECTION_DOWN;
    }

    if (GetDistanceToMario() < attackRange) {
        SetState(BossState::ATTACK);
    }

    if (GetDistanceToMario() > chaseRange) {
        SetState(BossState::PATROL);
    }
    if (GetDistanceToMario() > detectionRange) {
        SetState(BossState::IDLE);
    }
}

void Boss::Attack(float dt){
    if (attackTimer > 0) return; 

    //code logic tấn công ở đây

    attackTimer = attackCooldown;//reset
}

//đứng yên thấy mario thì đuổi theo
void Boss::Idle(float dt){
    vel.x = 0.0f;
    vel.y = 0.0f;

    if(CanSeeMario()){
        SetState(BossState::CHASE);
    } else {
        SetState(BossState::IDLE);
    }
}