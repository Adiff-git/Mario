#include "BossState.h"

string BossStateToString(BossState state){
    switch (state)
    {
        case BossState::IDLE: return "IDLE";
        case BossState::PATROL: return "PATROLLING";
        case BossState::CHASE: return "CHASING";
        case BossState::ATTACK: return "ATTACKING";
        case BossState::SKILL: return "SKILLING";
        case BossState::DIE: return "DEAD";
        default:
            return "UNKNOWN";
    }
}