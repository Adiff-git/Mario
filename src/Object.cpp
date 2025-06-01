#include "Object.h"

// full constructor 
Object::Object(Vector2 pos, Vector2 size, Vector2 vel, Color color, float frameTime, int maxFrames, Direction direction)
    : pos(pos), size(size), vel(vel), color(color), frameTime(frameTime), maxFrames(maxFrames), direction(direction),
      angle(0.0f), state(OBJECT_STATE_IDLE), AdditionalState(OBJECT_STATE_IDLE), sprite(nullptr),
      frameAcumulator(0.0f), currentFrame(0) {
    cpN = CollisionProbe{ Vector2{ pos.x + size.x/2 - 5, pos.y}, Vector2{size.x,5}, color }; // North probe
    cpS = CollisionProbe{ Vector2{ pos.x + size.x/2 - 5, pos.y + size.y - 5}, Vector2{size.x,5}, color }; // South probe
    cpE = CollisionProbe{ Vector2{ pos.x + size.x - 5, pos.y + size.y/2 - 5}, Vector2{5,size.y}, color }; // East probe
    cpW = CollisionProbe{ Vector2{ pos.x, pos.y + size.y/2 - 5}, Vector2{5,size.y}, color }; // West probe
}

Object::Object() : Object(Vector2{0, 0}, Vector2{50, 50}, Vector2{0, 0}, WHITE, 0, 0, DIRECTION_RIGHT) {
    // Default constructor initializes with default values
}


Object::Object(Vector2 pos, Vector2 size, Color color)
    : Object(pos, size, Vector2{0, 0}, color, 0.0f, 0, DIRECTION_RIGHT) {
    // Constructor with position, size and color
}

Object::Object(Vector2 pos, Vector2 size, Vector2 vel, Color color, float frameTime, int maxFrames)
    : Object(pos, size, vel, color, frameTime, maxFrames, DIRECTION_RIGHT) {
    // Constructor with position, size, velocity, color, frame time and max frames
}

Object::~Object() {
    // Destructor
    sprite = nullptr; // Assuming sprite is managed elsewhere
}

void Object::UpdateCollisionProbes() {
    // Update the position of collision probes based on the object's position and size
    cpN.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y });
    cpS.setPos(Vector2{ pos.x + size.x / 2 - 5, pos.y + size.y - 5 });
    cpE.setPos(Vector2{ pos.x + size.x - 5, pos.y + size.y / 2 - 5 });
    cpW.setPos(Vector2{ pos.x, pos.y + size.y / 2 - 5 });
}

void Object::UpdateStateAndPhysic() {
    // Update the object's state and physics based on its current state
    switch (state) {
        case OBJECT_STATE_JUMPING:
            vel.y -= 0.5f; // Simulate gravity
            break;
        case OBJECT_STATE_FALLING:
            vel.y += 0.5f; // Simulate gravity
            break;
        default:
            break;
    }
    
    pos.x += vel.x;
    pos.y += vel.y;
    UpdateCollisionProbes(); // Update collision probes after moving
}

CollisionType Object::checkCollision(const Object& other) const {
    // Check collision with another object
    Rectangle rectA = { pos.x, pos.y, size.x, size.y };
    Rectangle rectB = { other.pos.x, other.pos.y, other.size.x, other.size.y };
    
    if (CheckCollisionRecs(rectA, rectB)) {
        // Determine the type of collision based on the position of the probes
        if (cpN.checkCollision(rectB) == COLLISION_TYPE_NORTH) return COLLISION_TYPE_NORTH;
        if (cpS.checkCollision(rectB) == COLLISION_TYPE_SOUTH) return COLLISION_TYPE_SOUTH;
        if (cpE.checkCollision(rectB) == COLLISION_TYPE_EAST) return COLLISION_TYPE_EAST;
        if (cpW.checkCollision(rectB) == COLLISION_TYPE_WEST) return COLLISION_TYPE_WEST;
        return COLLISION_TYPE_COLLIDED; // General collision
    }
    return COLLISION_TYPE_NONE; // No collision
}

void Object::SetPos(Vector2 pos) {
    this->pos = pos;
    UpdateCollisionProbes(); // Update probes when position changes
}

void Object::SetSize(Vector2 size) {
    this->size = size;
    UpdateCollisionProbes(); // Update probes when size changes
}

void Object::SetAngle(float angle) {
    this->angle = angle;
}

void Object::SetState(ObjectState state) {
    this->state = state;
}

void Object::SetColor(Color color) {
    this->color = color;
    cpN.setColor(color);
    cpS.setColor(color);
    cpE.setColor(color);
    cpW.setColor(color);
}

void Object::SetDirection(Direction direction) {
    this->direction = direction;
}

void Object::SetVel(Vector2 vel) {
    this->vel = vel;
}

void Object::setFrameTime(float frameTime) {
    this->frameTime = frameTime;
}

void Object::setFrameAcum(float frameAcum) {
    this->frameAcumulator = frameAcum;
}

void Object::setCurrFrame(int currFrame) {
    this->currentFrame = currFrame;
}

void Object::setMaxFrame(int maxFrame) {
    this->maxFrames = maxFrame;
}

void Object::SetCollisionProbesTop(CollisionProbe Top) {
    cpN = Top;
}

void Object::SetCollisionProbesBottom(CollisionProbe Bottom) {
    cpS = Bottom;
}

void Object::SetCollisionProbesRight(CollisionProbe Right) {
    cpE = Right;
}

void Object::SetCollisionProbesLeft(CollisionProbe Left) {
    cpW = Left;
}

//Getters
Vector2& Object::GetPos() {
    return pos;
}

Vector2& Object::GetSize() {
    return size;
}

float Object::GetAngle() const {
    return angle;
}

ObjectState Object::GetState() const {
    return state;
}

Color& Object::GetColor() {
    return color;
}

Direction Object::GetDirection() const {
    return direction;
}

Vector2& Object::GetVel() {
    return vel;
}

float Object::GetFrameTime() const {
    return frameTime;
}
float Object::GetFrameAcum() const {
    return frameAcumulator;
}
int Object::GetCurrFrame() const {
    return currentFrame;
}
int Object::GetMaxFrame() const {
    return maxFrames;
}