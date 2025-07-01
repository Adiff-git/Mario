#include "raylib.h"
#include "CollisionProbe.h"
#include "SpriteState.h"
#include "Direction.h"
#include "CollisionType.h"
#include "GameWorld.h"

class Sprite {
protected:
    Vector2 pos;
    Vector2 dim;//kích thước
    Vector2 vel;//vận tốc
    Color color;
    float frameTime;
    float frameAcum;
    int currentFrame;
    int maxFrames;
    int earnedPoints;
    SpriteState state;

    CollisionProbe cpN, cpS, cpE, cpW;

public:
    Sprite();
    Sprite(Vector2 pos, Vector2 dim, Vector2 vel, Color color,
       float frameTime, int maxFrames, Direction facingDirection,
       int hitsToDie, int earnedPoints);
    virtual ~Sprite() = default;

    void setPos(Vector2 pos);
    void setVel(Vector2 vel);
    Vector2& getPos();
    Vector2& getVel();
    Vector2& getDim();
    Color& getColor();
    Rectangle getRect() const;
    SpriteState getState() const;
    void setState(SpriteState state);
    void updateCollisionProbes();
    virtual CollisionType checkCollision(Sprite* other);

    virtual void update() = 0;
    virtual void draw() = 0;
};