#ifndef BUZZY_BEETLE_H
#define BUZZY_BEETLE_H

#include "Enemy.h"
#include "ResrcManager.h"

class BuzzyBeetle : public Enemy
{
public:
    BuzzyBeetle(Vector2 pos);
    void UpdateStateAndPhysic() override;
};

#endif