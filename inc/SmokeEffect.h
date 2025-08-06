#pragma once
#include "Object.h"
#include "ResrcManager.h"
#include <vector>
#include "../inc/World/GameClock.h"

class SmokeEffect : public Object {
protected:  // Đổi từ private thành protected để cho phép kế thừa
    std::vector<Texture2D*> smokeFrames;
    int currentFrame;
    int maxFrames;
    float frameTime;
    float frameAcumulator;
    float lifeTime;
    float lifeTimeAcum;
    float alpha;
    bool isActive;

public:
    SmokeEffect(Vector2 pos);
    ~SmokeEffect() = default;
    
    void Update() override;
    void Draw() override;
    void UpdateStateAndPhysic() override;
    
    bool IsActive() const { return isActive; }
    bool ShouldRemove() const { return lifeTimeAcum >= lifeTime; }
};