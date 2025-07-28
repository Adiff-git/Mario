#pragma once
#include "raylib.h"
#include "Object.h"
#include "Enemy.h"
#include "BossState.h"
#include "BehaviorTree.h"
#include "Mario.h"
#include "ResrcManager.h"
#include "BossFireball.h"
#include <memory>
#include <cmath>
#include <list>
using namespace std;


class Boss : public Enemy{
private:
    BossState currentState;//FSM

    BehaviorTree* behavior;//Behavior tree

    Vector2* marioPos;

    Texture2D* currentTexture;  // Texture hiện tại đang dùng
    
    vector<Texture2D*> idleFrames;    
    vector<Texture2D*> movingFrames;  
    vector<Texture2D*> chaseFrames;   
    vector<Texture2D*> attackFrames;
    vector<Texture2D*> skillFlyFrames; 
    //tự thêm mấy skill kia dô đây  
    
    int skillCurrentFrame;           // Frame hiện tại của skill
    float skillFrameTime;           // Thời gian giữa các skill frame
    float skillFrameAccumulator;    // Đếm thời gian skill frame
    float skillDurationAccumulator; // Track skill duration (instance variable)
    
    float detectionRange;  // Phát hiện Mario
    float chaseRange;      // Bắt đầu đuổi
    float attackRange;     // Bắt đầu tấn công
    float moveSpeed;       // Tốc độ di chuyển
    float chaseSpeedMultiplier; // Nhân tốc độ khi chase

    float attackCooldown;   // Thời gian hồi attack
    float attackTimer;      // Đếm ngược hồi attack
    
    int attackCount;        // Đếm số lần attack liên tiếp
    int maxAttacks;         // Số attack tối đa trước khi nghỉ
    float skillCooldown;    // Thời gian hồi skill
    float skillTimer;       // Đếm ngược skill
    bool isUsingSkill;      // Đang dùng skill hay không

    float patrolTimer;
    int patrolPhase;
    
    // Hit counter system
    int hitCount;           // Number of times Boss has been hit by Mario's fireballs
    static const int maxHits = 10; // Boss dies after 10 hits
    float hitCooldown;      // Cooldown timer to prevent multiple hits per frame
    static constexpr float hitCooldownTime = 0.5f; // 0.5 second cooldown between hits
    
    // Projectile management
    std::list<BossFireball*> projectiles;
    
    void OnStateEnter(BossState newState);
    
    // Helper để khởi tạo behavior tree
    void BuildBehaviorTree();
    
    // Helper để load textures
    void LoadTextures();
    
    // Helper để update texture theo state
    void UpdateTexture();
public:
    Boss(Vector2 startPos, Vector2* marioPosition);
    ~Boss();

    void Update();
    void UpdateStateAndPhysic() override; // Implement pure virtual từ Enemy
    void Draw();

    // ===== FSM control =====
    void SetState(BossState newState);
    BossState GetState() const { return currentState; }

    // ===== Điều kiện cho Behavior Tree =====
    bool CanSeeMario() const;
    bool IsCloseToMario() const;

    // ===== Action methods cho FSM =====
    void Patrol(float dt);
    void Chase(float dt);
    void Attack(float dt);
    void Idle(float dt); 
    void UseSkill(float dt);

    // ===== Utility =====
    float GetDistanceToMario() const;
    Vector2 GetDirectionToMario() const;
    Vector2 GetPredictedDirectionToMario() const;
    void FireProjectile(Vector2 direction);
    
    // ===== Projectile management =====
    void UpdateProjectiles();
    void DrawProjectiles();
    void CleanupProjectiles();
    std::list<BossFireball*>& getProjectiles() { return projectiles; }
    
    // ===== Getters & Setters =====
    bool IsSkillReady() const { return skillTimer <= 0 && !isUsingSkill; }
    void SetMarioPosition(Vector2* marioPosition) { marioPos = marioPosition; }
    
    // ===== Hit counter methods =====
    void OnHitByFireball();
    int GetHitCount() const { return hitCount; }
    bool IsDead() const { return hitCount >= maxHits; }

};