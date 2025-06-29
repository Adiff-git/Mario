#include "CollisionProbe.h"
#include "raylib.h"

CollisionProbe::CollisionProbe() : pos{0, 0}, size{5, 5}, color{WHITE} {
    // Default constructor initializes with default values
}

CollisionProbe::CollisionProbe(Vector2 pos, Vector2 size, Color color) 
    : pos(pos), size(size), color(color) {
    // Constructor with position, size and color
}

CollisionProbe::~CollisionProbe() {
    // Destructor
}

void CollisionProbe::draw() {
    DrawRectangleV(pos, size, color);
}

CollisionType CollisionProbe::checkCollision(const Rectangle &rect) const {
    Rectangle probeRect = { pos.x, pos.y, size.x, size.y };
    
    if (CheckCollisionRecs(probeRect, rect)) {
        // Xác định phương va chạm dựa trên vị trí tương đối
        
        // Tính các cạnh và độ xâm nhập
        float leftPenetration = (rect.x + rect.width) - probeRect.x;                // Bên trái xâm nhập bao nhiêu
        float rightPenetration = (probeRect.x + probeRect.width) - rect.x;          // Bên phải xâm nhập bao nhiêu
        float topPenetration = (rect.y + rect.height) - probeRect.y;                // Phía trên xâm nhập bao nhiêu
        float bottomPenetration = (probeRect.y + probeRect.height) - rect.y;        // Phía dưới xâm nhập bao nhiêu
        
        // Tìm hướng va chạm dựa trên độ xâm nhập nhỏ nhất
        float minPenetration = leftPenetration;
        CollisionType collisionType = COLLISION_TYPE_WEST;
        
        if (rightPenetration < minPenetration) {
            minPenetration = rightPenetration;
            collisionType = COLLISION_TYPE_EAST;
        }
        
        if (topPenetration < minPenetration) {
            minPenetration = topPenetration;
            collisionType = COLLISION_TYPE_NORTH;
        }
        
        if (bottomPenetration < minPenetration) {
            collisionType = COLLISION_TYPE_SOUTH;
        }
        
        return collisionType;
    }
    
    return COLLISION_TYPE_NONE;
}

void CollisionProbe::setPos(Vector2 pos) {
    this->pos = pos;
}
void CollisionProbe::setPos(float x, float y) {
    this->pos = {x, y};
}
void CollisionProbe::setX(float x) {
    this->pos.x = x;
}
void CollisionProbe::setY(float y) {
    this->pos.y = y;
}

void CollisionProbe::setSize(float width, float height) {
    this->size = {width, height};
}
void CollisionProbe::setSize(Vector2 dim) {
    size = dim;
}
void CollisionProbe::setWidth(float width) {
    this->size.x = width;
}
void CollisionProbe::setHeight(float height) {
    this->size.y = height;
}
void CollisionProbe::setColor(Color color) {
    this->color = color;
}
Vector2& CollisionProbe::getPos() {
    return pos;
}
float CollisionProbe::getX() const {
    return pos.x;
}
float CollisionProbe::getY() const {
    return pos.y;
}
Vector2& CollisionProbe::getSize() {
    return size;
}
float CollisionProbe::getWidth() const {
    return size.x;
}
float CollisionProbe::getHeight() const {
    return size.y;
}
Color& CollisionProbe::getColor() {
    return color;
}
Rectangle CollisionProbe::getRect() const {
    return { pos.x, pos.y, size.x, size.y };
}

