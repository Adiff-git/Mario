#include "BTNodes.h"

SelectorNode::SelectorNode(vector<BTNode*> nodes) : children(nodes) {}

BTStatus SelectorNode::Execute() {
    for (auto& child : children) {
        BTStatus status = child->Execute();
        if (status != BTStatus::FAILURE) {
            return status; 
        }
    }
    return BTStatus::FAILURE; 
}

SequenceNode::SequenceNode(vector<BTNode*> nodes) : children(nodes) {}

BTStatus SequenceNode::Execute() {
    for (auto& child : children) {
        BTStatus status = child->Execute();
        if (status == BTStatus::FAILURE) {
            return status;
        }
    }
    return BTStatus::SUCCESS;
}

ConditionNode::ConditionNode(function<bool()> cond) : condition(cond) {}

BTStatus ConditionNode::Execute() {
    return condition() ? BTStatus::SUCCESS : BTStatus::FAILURE;
}

ActionNode::ActionNode(function<void()> act) : action(act) {}

BTStatus ActionNode::Execute() {
    action();
    return BTStatus::SUCCESS;// để BT chuyển bước tiếp theo
}