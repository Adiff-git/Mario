#pragma once
#include <functional>
#include <vector>
using namespace std;

enum class BTStatus{
    SUCCESS,
    FAILURE,
    RUNNING
};

class BTNode {
public:
    virtual ~BTNode() = default;
    virtual BTStatus Execute() = 0;
};

class SelectorNode : public BTNode {
protected:
    vector<BTNode*> children;

public:
    explicit SelectorNode(vector<BTNode*> nodes);
    BTStatus Execute() override;
};

class SequenceNode : public BTNode {
protected:
    vector<BTNode*> children;

public:
    explicit SequenceNode(vector<BTNode*> nodes);//constructor
    BTStatus Execute() override;
};

//node check điều kiện
class ConditionNode : public BTNode {
protected:
    function<bool()> condition;

public:
    explicit ConditionNode(function<bool()> cond);
    BTStatus Execute() override;
};

//node hành động
class ActionNode : public BTNode {
protected:
    function<void()> action;

public:
    explicit ActionNode(function<void()> act);
    BTStatus Execute() override;
};

