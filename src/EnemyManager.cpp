#include "EnemyManager.h"
#include "Goomba.h"
#include "GreenKoopa.h"  
#include "YellowKoopa.h"
#include "BuzzyBeetle.h"
#include "FlyingGoomba.h"
#include "PiranhaPlant.h"
#include "JumpingPiranha.h"
#include "BulletBill.h"
#include "BanzaiBill.h"
#include "Bob-omb.h" 
#include "Rex.h"    
#include <stdexcept>
#include <iostream>     

EnemyManager::EnemyManager() {}

EnemyManager::~EnemyManager() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

Enemy* EnemyManager::CreateEnemy(const std::string& enemyType, Vector2 pos) {
    Enemy* enemy = nullptr;
    std::cout << "Attempting to create " << enemyType << " at (" << pos.x << ", " << pos.y << ")" << std::endl; // Debug
    if (enemyType == "Goomba") {
        enemy = new Goomba(pos);
    } else if (enemyType == "GreenKoopa") {
        enemy = new GreenKoopa(pos);
    } else if (enemyType == "YellowKoopa") {
        enemy = new YellowKoopa(pos);
    } else if (enemyType == "BuzzyBeetle") {
        enemy = new BuzzyBeetle(pos);
    } else if (enemyType == "FlyingGoomba") {
        enemy = new FlyingGoomba(pos);
    } else if (enemyType == "PiranhaPlant") {
        enemy = new PiranhaPlant(pos);
    } else if (enemyType == "JumpingPiranhaPlant") {
        enemy = new JumpingPiranhaPlant(pos);
    } else if (enemyType == "BulletBill") {
        enemy = new BulletBill(pos);
    } else if (enemyType == "BanzaiBill") {
        enemy = new BanzaiBill(pos);
    } else if (enemyType == "Bob-omb") {
        enemy = new Bob_omb(pos);
    }else if (enemyType=="Rex"){
        enemy=new Rex(pos);
    }
     else {
        throw std::runtime_error("Unknown enemy type: " + enemyType);
    }
    if (enemy) {
        enemies.push_back(enemy);
        std::cout << "Successfully created " << enemyType << ", enemy count: " << enemies.size() << std::endl;
    } else {
        std::cout << "Failed to create " << enemyType << std::endl;
    }
    return enemy;
}

void EnemyManager::Update() {
    for (Enemy* enemy : enemies) {
        enemy->Update();
    }
}

std::vector<Enemy*>& EnemyManager::GetEnemies() {
    return enemies;
}