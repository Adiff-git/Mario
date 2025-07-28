#ifndef ENEMY_MANAGER_H
#define ENEMY_MANAGER_H
#include "Enemy.h"
#include <vector>
#include <string>

class EnemyManager {
private:
    std::vector<Enemy*> enemies;

public:
    EnemyManager();
    ~EnemyManager();
    Enemy* CreateEnemy(const std::string& enemyType, Vector2 pos);
    void Update();
    std::vector<Enemy*>& GetEnemies();
};

#endif