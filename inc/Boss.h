#pragma once
#include "raylib.h"
#include "Object.h"
#include "Enemy.h"
#include "BossState.h"
#include "BehaviorTree.h"
#include "Mario.h"
#include "ResrcManager.h"
#include <memory>
#include <cmath>
using namespace std;


class Boss : public Enemy{
private:
    BossState currentState;//FSM

    BehaviorTree* behavior;//Behavior tree

    Vector2* marioPos;//check mario position

    // ===== Textures =====
    Texture2D* currentTexture;  // Texture hiện tại đang dùng
    
    // Skill animation frames cho từng state
    vector<Texture2D*> idleFrames;    // Skill 1 frames cho IDLE
    vector<Texture2D*> chaseFrames;   // Skill 2 frames cho CHASE/PATROL  
    vector<Texture2D*> attackFrames;  // Skill 3 frames cho ATTACK
    vector<Texture2D*> skillFrames;   // Skill 4,6 frames cho SKILL (fire)
    
    int skillCurrentFrame;           // Frame hiện tại của skill
    float skillFrameTime;           // Thời gian giữa các skill frame
    float skillFrameAccumulator;    // Đếm thời gian skill frame
    
    // ===== Các tham số AI =====
    float detectionRange;  // Phát hiện Mario
    float chaseRange;      // Bắt đầu đuổi
    float attackRange;     // Bắt đầu tấn công
    float moveSpeed;       // Tốc độ di chuyển
    float chaseSpeedMultiplier; // Nhân tốc độ khi chase

    // ===== Cooldown =====
    float attackCooldown;   // Thời gian hồi attack
    float attackTimer;      // Đếm ngược hồi attack
    
    // ===== Attack System =====
    int attackCount;        // Đếm số lần attack liên tiếp
    int maxAttacks;         // Số attack tối đa trước khi nghỉ
    float skillCooldown;    // Thời gian hồi skill
    float skillTimer;       // Đếm ngược skill
    bool isUsingSkill;      // Đang dùng skill hay không
    
    // Helper để reset animation hoặc timer khi đổi state
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
    void FireProjectile(Vector2 direction);
    
    // ===== Getters & Setters =====
    bool IsSkillReady() const { return skillTimer <= 0 && !isUsingSkill; }
    void SetMarioPosition(Vector2* marioPosition) { marioPos = marioPosition; }

};