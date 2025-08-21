#include "BehaviorTree.h"

BehaviorTree::BehaviorTree(BTNode* rootNode) : root(rootNode) {}
BehaviorTree::~BehaviorTree() {
    delete root; 
}

void BehaviorTree::Tick() {
    if (root) {
        root->Execute();
    }
}
//thienlyoi