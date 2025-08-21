#pragma once
#include <string>
#include "ResrcManager.h"
using namespace std;

enum class BossState{
    IDLE,       // Đứng yên
    PATROL,     // Đi tuần
    CHASE,      // Đuổi Mario
    ATTACK,     // Tấn công
    SKILL,      
    DIE 
};

string BossStateToString(BossState state);