#pragma once
#include "BossState.h"
#include "BehaviorTree.h"
#include <raylib.h> 
#include <memory>
#include "Object.h"
#include "Mario.h"
#include <cmath>
using namespace std;


class Boss : public Object{
private:
    BossState currentState;//FSM

    BehaviorTree* behavior;//Behavior tree

    Vector2* marioPos;//check mario position

    // ===== Các tham số AI =====
    float detectionRange;  // Phát hiện Mario
    float chaseRange;      // Bắt đầu đuổi
    float attackRange;     // Bắt đầu tấn công
    float moveSpeed;       // Tốc độ di chuyển

    // ===== Cooldown =====
    float attackCooldown;   // Thời gian hồi attack
    float attackTimer;      // Đếm ngược hồi attack
    
    // Helper để reset animation hoặc timer khi đổi state
    void OnStateEnter(BossState newState);
    
    // Helper để khởi tạo behavior tree
    void BuildBehaviorTree();
public:
    Boss(Vector2 startPos, Vector2* marioPosition);
    ~Boss();

    void Update();
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

    // ===== Utility =====
    float GetDistanceToMario() const;

};