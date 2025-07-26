#pragma once
#include <string>
#include "ResrcManager.h"
using namespace std;

enum class BossState{
    IDLE,       // Đứng yên
    PATROL,     // Đi tuần
    CHASE,      // Đuổi Mario
    ATTACK,     // Tấn công
    SKILL       // Sill bắn 3 lần mới chết
};

//debug
string BossStateToString(BossState state);