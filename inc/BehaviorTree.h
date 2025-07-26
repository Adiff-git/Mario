#pragma once
#include "BTNodes.h"

class BehaviorTree {
private:
    BTNode* root;

public:
    explicit BehaviorTree(BTNode* rootNode);
    ~BehaviorTree();
    void Tick();
};