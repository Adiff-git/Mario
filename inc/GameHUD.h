#pragma once

#include "Drawable.h"
#include "Mario.h"

class GameHUD : public Drawable {
    private:
        Mario* mario;
        Mario* luigi;

        Vector2 coinsPosition;
        Vector2 coinsSize;
        Texture2D *coinTexture;

        Vector2 scorePosition;
        Vector2 scoreSize;
        Texture2D *MarioHUD;

        Font* font;
    public:
        GameHUD() = default;
        GameHUD(Mario* mario);
        GameHUD(Mario* mario, Mario* luigi);// for multiple player

        void Draw() override;
        void setFont(Font& font);
        void setTextColor(Color color);
};