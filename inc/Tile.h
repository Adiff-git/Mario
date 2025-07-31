#pragma once

#include "Object.h"
#include "ResrcManager.h"
#include <string>

class Tile : public Object {
    private:
        void Update() override{};
        int key;
    public:
        Tile(Vector2 pos,int map,  int key);
        void Draw() override;
        int GetKey() const { return key; }
        
    };