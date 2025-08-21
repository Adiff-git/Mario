#pragma once
#include <string>
#include "ResrcManager.h"
using namespace std;

enum class BossState{
    IDLE,       // Đứng yên
    PATROL,     // Đi tuần
    CHASE,      // Đuổi Mario
    ATTACK,     // Tấn công
    SKILL,      // Skill nhảy
    DIE // Trạng thái chết
};

string BossStateToString(BossState state);