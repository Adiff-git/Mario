#pragma once
#include "../inc/Enemy/Enemy.h"

// Example method to demonstrate how to use ScoreEffect
// Call this when an enemy is defeated (e.g., when player jumps on it or hits with fireball)
inline void DemoScoreEffect(Enemy* enemy) {
    if (enemy && enemy->GetState() != OBJECT_STATE_DYING && enemy->GetState() != OBJECT_STATE_DEAD) {
        // Tạo điểm số dựa trên loại enemy
        int score = 100; // Default score for Goomba
        
        // Có thể customized score dựa trên enemy type:
        // if (dynamic_cast<Goomba*>(enemy)) score = 100;
        // if (dynamic_cast<GreenKoopa*>(enemy)) score = 200;
        // if (dynamic_cast<RedKoopa*>(enemy)) score = 400;
        // if (dynamic_cast<BuzzyBeetle*>(enemy)) score = 800;
        
        // Tạo score effect
        enemy->CreateScoreEffect(score);
        
        // Set enemy state to dying
        enemy->SetState(OBJECT_STATE_DYING);
        enemy->CreateDeathEffect(); // Tạo smoke effect cũ
    }
}

// Example usage in collision detection:
/*
void HandlePlayerEnemyCollision(Player* player, Enemy* enemy) {
    if (player->IsJumping() && player->GetVel().y > 0) {
        // Player jumped on enemy
        DemoScoreEffect(enemy);
        player->Bounce(); // Make player bounce up
    } else {
        // Player hit by enemy - take damage
        player->TakeDamage();
    }
}
*/
