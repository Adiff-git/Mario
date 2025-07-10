#include "Enemy.h"
class PiranhaPlant : public Enemy {
private:
    float animationTimer = 0.0f;
    float animationInterval = 0.5f; 
    bool isMouthOpen = true;

public:
    PiranhaPlant(Vector2 pos);
    void UpdateStateAndPhysic() override;
};