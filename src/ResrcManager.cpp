#include "ResrcManager.h" 

ResrcManager* ResrcManager::instance = nullptr;

ResrcManager::~ResrcManager() {
    // Clean up resources if necessary
    unloadResources();
    
    instance = nullptr;
}

void ResrcManager::loadTextures() {
    
    {
        // SMall Mario textures
        {
        textures["SMALLMARIO_0_RIGHT"] = LoadTexture("resources/images/Object/mario/SmallMario_0.png");
        textures["SMALLMARIO_0_LEFT"] = FlipTextureHorizontal(textures["SMALLMARIO_0_RIGHT"]);

        textures["SMALLMARIO_1_RIGHT"] = LoadTexture("resources/images/Object/mario/SmallMario_1.png");
        textures["SMALLMARIO_1_LEFT"] = FlipTextureHorizontal(textures["SMALLMARIO_1_RIGHT"]);

        textures["SMALLMARIO_JUMPING_RIGHT"] = LoadTexture("resources/images/Object/mario/SmallMarioJumping_0.png");
        textures["SMALLMARIO_JUMPING_LEFT"] = FlipTextureHorizontal(textures["SMALLMARIO_JUMPING_RIGHT"]);
        textures["SMALLMARIO_DUCKING_RIGHT"] = LoadTexture("resources/images/Object/mario/SmallMarioDucking_0.png");
        textures["SMALLMARIO_DUCKING_LEFT"] = FlipTextureHorizontal(textures["SMALLMARIO_DUCKING_RIGHT"]);
        textures["SMALLMARIO_FALLING_RIGHT"] = LoadTexture("resources/images/Object/mario/SmallMarioFalling_0.png");
        textures["SMALLMARIO_FALLING_LEFT"] = FlipTextureHorizontal(textures["SMALLMARIO_FALLING_RIGHT"]);
        textures["MARIO_DIE"] = LoadTexture("resources/images/Object/mario/SmallMarioDying_0.png");
        textures["SMALLMARIO_VICTORY"] = LoadTexture("resources/images/Object/mario/SmallMarioVictory_0.png");
        }

        textures["SUPER_MARIO_0_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMario_0.png");
        textures["SUPER_MARIO_1_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMario_1.png");
        textures["SUPER_MARIO_2_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMario_2.png");
        textures["SUPER_MARIO_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_0_RIGHT"]);
        textures["SUPER_MARIO_1_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_1_RIGHT"]);
        textures["SUPER_MARIO_2_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_2_RIGHT"]);

        textures["SUPER_MARIO_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMarioJumping_0.png");
        textures["SUPER_MARIO_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_JUMPING_0_RIGHT"]);
        textures["SUPER_MARIO_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMarioFalling_0.png");
        textures["SUPER_MARIO_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_FALLING_0_RIGHT"]);

        textures["SUPER_MARIO_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/SuperMarioDucking_0.png");
        textures["SUPER_MARIO_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_MARIO_DUCKING_0_RIGHT"]);

        
        textures["SUPER_MARIO_VICTORY"] = LoadTexture("resources/images/Object/mario/SuperMarioVictory_0.png");

        textures["TRANSITIONING_MARIO_0_RIGHT"] = LoadTexture("resources/images/Object/mario/TransitioningMario_0.png");
        textures["TRANSITIONING_MARIO_0_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_MARIO_0_RIGHT"]);
        textures["TRANSITIONING_MARIO_1_RIGHT"] = LoadTexture("resources/images/Object/mario/TransitioningMario_1.png");
        textures["TRANSITIONING_MARIO_1_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_MARIO_1_RIGHT"]);
        textures["TRANSITIONING_MARIO_2_RIGHT"] = LoadTexture("resources/images/Object/mario/TransitioningMario_2.png");
        textures["TRANSITIONING_MARIO_2_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_MARIO_2_RIGHT"]);
        //FIRE MARIO
        textures["FIRE_MARIO_0_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMario_0.png");
        textures["FIRE_MARIO_1_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMario_1.png");
        textures["FIRE_MARIO_2_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMario_2.png");
        textures["FIRE_MARIO_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_0_RIGHT"]);
        textures["FIRE_MARIO_1_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_1_RIGHT"]);
        textures["FIRE_MARIO_2_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_2_RIGHT"]);

        textures["FIRE_MARIO_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMarioJumping_0.png");
        textures["FIRE_MARIO_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_JUMPING_0_RIGHT"]);
        textures["FIRE_MARIO_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMarioFalling_0.png");
        textures["FIRE_MARIO_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_FALLING_0_RIGHT"]);

        textures["FIRE_MARIO_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/mario/FireMario_Ducking_0.png");
        textures["FIRE_MARIO_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_MARIO_DUCKING_0_RIGHT"]);
        textures["FIRE_MARIO_VICTORY"] = LoadTexture("resources/images/Object/mario/FireMarioVictory.png");

        {
        textures["FIRE_BALL_0_RIGHT"] = LoadTexture("resources/images/Object/mario/FlowerMarioFireball_0.png");
        textures["FIRE_BALL_1_RIGHT"] = LoadTexture("resources/images/Object/mario/FlowerMarioFireball_1.png");
        textures["FIRE_BALL_2_RIGHT"] = LoadTexture("resources/images/Object/mario/FlowerMarioFireball_2.png");
        textures["FIRE_BALL_3_RIGHT"] = LoadTexture("resources/images/Object/mario/FlowerMarioFireball_3.png");
        textures["FIRE_BALL_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_BALL_0_RIGHT"]);
        textures["FIRE_BALL_1_LEFT"] = FlipTextureHorizontal(textures["FIRE_BALL_1_RIGHT"]);
        textures["FIRE_BALL_2_LEFT"] = FlipTextureHorizontal(textures["FIRE_BALL_2_RIGHT"]);
        textures["FIRE_BALL_3_LEFT"] = FlipTextureHorizontal(textures["FIRE_BALL_3_RIGHT"]);
        }
    }
    
    {
        // Small Luigi textures
        {
            textures["SMALLLUIGI_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/SmallLuigi_0.png");
            textures["SMALLLUIGI_0_LEFT"] = FlipTextureHorizontal(textures["SMALLLUIGI_0_RIGHT"]);

            textures["SMALLLUIGI_1_RIGHT"] = LoadTexture("resources/images/Object/luigi/SmallLuigi_1.png");
            textures["SMALLLUIGI_1_LEFT"] = FlipTextureHorizontal(textures["SMALLLUIGI_1_RIGHT"]);

            textures["SMALLLUIGI_JUMPING_RIGHT"] = LoadTexture("resources/images/Object/luigi/SmallLuigiJumping_0.png");
            textures["SMALLLUIGI_JUMPING_LEFT"] = FlipTextureHorizontal(textures["SMALLLUIGI_JUMPING_RIGHT"]);
            textures["SMALLLUIGI_DUCKING_RIGHT"] = LoadTexture("resources/images/Object/luigi/SmallLuigiDucking_0.png");
            textures["SMALLLUIGI_DUCKING_LEFT"] = FlipTextureHorizontal(textures["SMALLLUIGI_DUCKING_RIGHT"]);
            textures["SMALLLUIGI_FALLING_RIGHT"] = LoadTexture("resources/images/Object/luigi/SmallLuigiFalling_0.png");
            textures["SMALLLUIGI_FALLING_LEFT"] = FlipTextureHorizontal(textures["SMALLLUIGI_FALLING_RIGHT"]);
            textures["LUIGI_DIE"] = LoadTexture("resources/images/Object/luigi/SmallLuigiDying_0.png");
            textures["SMALLLUIGI_VICTORY"] = LoadTexture("resources/images/Object/luigi/SmallLuigiVictory_0.png");
        }

        textures["SUPER_LUIGI_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigi_0.png");
        textures["SUPER_LUIGI_1_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigi_1.png");
        textures["SUPER_LUIGI_2_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigi_2.png");
        textures["SUPER_LUIGI_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_0_RIGHT"]);
        textures["SUPER_LUIGI_1_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_1_RIGHT"]);
        textures["SUPER_LUIGI_2_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_2_RIGHT"]);

        textures["SUPER_LUIGI_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigiJumping_0.png");
        textures["SUPER_LUIGI_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_JUMPING_0_RIGHT"]);
        textures["SUPER_LUIGI_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigiFalling_0.png");
        textures["SUPER_LUIGI_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_FALLING_0_RIGHT"]);

        textures["SUPER_LUIGI_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/SuperLuigiDucking_0.png");
        textures["SUPER_LUIGI_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_LUIGI_DUCKING_0_RIGHT"]);

        textures["SUPER_LUIGI_VICTORY"] = LoadTexture("resources/images/Object/luigi/SuperLuigiVictory_0.png");

        textures["TRANSITIONING_LUIGI_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/TransitioningLuigi_0.png");
        textures["TRANSITIONING_LUIGI_0_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_LUIGI_0_RIGHT"]);
        textures["TRANSITIONING_LUIGI_1_RIGHT"] = LoadTexture("resources/images/Object/luigi/TransitioningLuigi_1.png");
        textures["TRANSITIONING_LUIGI_1_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_LUIGI_1_RIGHT"]);
        textures["TRANSITIONING_LUIGI_2_RIGHT"] = LoadTexture("resources/images/Object/luigi/TransitioningLuigi_2.png");
        textures["TRANSITIONING_LUIGI_2_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_LUIGI_2_RIGHT"]);

        //FIRE LUIGI
        textures["FIRE_LUIGI_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigi_0.png");
        textures["FIRE_LUIGI_1_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigi_1.png");
        textures["FIRE_LUIGI_2_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigi_2.png");
        textures["FIRE_LUIGI_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_0_RIGHT"]);
        textures["FIRE_LUIGI_1_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_1_RIGHT"]);
        textures["FIRE_LUIGI_2_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_2_RIGHT"]);

        textures["FIRE_LUIGI_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigiJumping_0.png");
        textures["FIRE_LUIGI_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_JUMPING_0_RIGHT"]);
        textures["FIRE_LUIGI_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigiFalling_0.png");
        textures["FIRE_LUIGI_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_FALLING_0_RIGHT"]);

        textures["FIRE_LUIGI_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/luigi/FlowerLuigi_Ducking_0.png");
        textures["FIRE_LUIGI_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_LUIGI_DUCKING_0_RIGHT"]);
        textures["FIRE_LUIGI_VICTORY"] = LoadTexture("resources/images/Object/luigi/FlowerLuigiVictory.png");
    }
    
    // PEACH TEXTURES



    {


        // Small Peach textures


        textures["SMALLPEACH_0_RIGHT"] = LoadTexture("resources/images/Object/peach/SmallPeach_0.png");


        textures["SMALLPEACH_0_LEFT"] = FlipTextureHorizontal(textures["SMALLPEACH_0_RIGHT"]);





        textures["SMALLPEACH_1_RIGHT"] = LoadTexture("resources/images/Object/peach/SmallPeach_1.png");


        textures["SMALLPEACH_1_LEFT"] = FlipTextureHorizontal(textures["SMALLPEACH_1_RIGHT"]);





        textures["SMALLPEACH_JUMPING_RIGHT"] = LoadTexture("resources/images/Object/peach/SmallPeachJumping_0.png");


        textures["SMALLPEACH_JUMPING_LEFT"] = FlipTextureHorizontal(textures["SMALLPEACH_JUMPING_RIGHT"]);


        textures["SMALLPEACH_DUCKING_RIGHT"] = LoadTexture("resources/images/Object/peach/SmallPeachDucking_0.png");


        textures["SMALLPEACH_DUCKING_LEFT"] = FlipTextureHorizontal(textures["SMALLPEACH_DUCKING_RIGHT"]);


        textures["SMALLPEACH_FALLING_RIGHT"] = LoadTexture("resources/images/Object/peach/SmallPeachFalling_0.png");


        textures["SMALLPEACH_FALLING_LEFT"] = FlipTextureHorizontal(textures["SMALLPEACH_FALLING_RIGHT"]);


        textures["PEACH_DIE"] = LoadTexture("resources/images/Object/peach/SmallPeachDying_0.png");


        textures["SMALLPEACH_VICTORY"] = LoadTexture("resources/images/Object/peach/SmallPeachVictory_0.png");





        // Super Peach textures


        textures["SUPER_PEACH_0_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeach_0.png");


        textures["SUPER_PEACH_1_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeach_1.png");


        textures["SUPER_PEACH_2_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeach_2.png");


        textures["SUPER_PEACH_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_0_RIGHT"]);


        textures["SUPER_PEACH_1_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_1_RIGHT"]);


        textures["SUPER_PEACH_2_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_2_RIGHT"]);





        textures["SUPER_PEACH_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeachJumping_0.png");


        textures["SUPER_PEACH_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_JUMPING_0_RIGHT"]);


        textures["SUPER_PEACH_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeachFalling_0.png");


        textures["SUPER_PEACH_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_FALLING_0_RIGHT"]);





        textures["SUPER_PEACH_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/SuperPeachDucking_0.png");


        textures["SUPER_PEACH_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_PEACH_DUCKING_0_RIGHT"]);





        textures["SUPER_PEACH_VICTORY"] = LoadTexture("resources/images/Object/peach/SuperPeachVictory_0.png");





        // Transitioning Peach textures


        textures["TRANSITIONING_PEACH_0_RIGHT"] = LoadTexture("resources/images/Object/peach/TransitioningPeach_0.png");


        textures["TRANSITIONING_PEACH_0_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_PEACH_0_RIGHT"]);


        textures["TRANSITIONING_PEACH_1_RIGHT"] = LoadTexture("resources/images/Object/peach/TransitioningPeach_1.png");


        textures["TRANSITIONING_PEACH_1_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_PEACH_1_RIGHT"]);


        textures["TRANSITIONING_PEACH_2_RIGHT"] = LoadTexture("resources/images/Object/peach/TransitioningPeach_2.png");


        textures["TRANSITIONING_PEACH_2_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_PEACH_2_RIGHT"]);





        // Fire Peach textures


        textures["FIRE_PEACH_0_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeach_0.png");


        textures["FIRE_PEACH_1_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeach_1.png");


        textures["FIRE_PEACH_2_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeach_2.png");


        textures["FIRE_PEACH_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_0_RIGHT"]);


        textures["FIRE_PEACH_1_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_1_RIGHT"]);


        textures["FIRE_PEACH_2_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_2_RIGHT"]);





        textures["FIRE_PEACH_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeachJumping_0.png");


        textures["FIRE_PEACH_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_JUMPING_0_RIGHT"]);


        textures["FIRE_PEACH_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeachFalling_0.png");


        textures["FIRE_PEACH_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_FALLING_0_RIGHT"]);





        textures["FIRE_PEACH_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/peach/FirePeachDucking_0.png");


        textures["FIRE_PEACH_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_PEACH_DUCKING_0_RIGHT"]);


        textures["FIRE_PEACH_VICTORY"] = LoadTexture("resources/images/Object/peach/FirePeachVictory_0.png");


    }


    // TOAD TEXTURES
    {


        // Small Toad textures


        textures["SMALLTOAD_0_RIGHT"] = LoadTexture("resources/images/Object/toad/SmallToad_0.png");


        textures["SMALLTOAD_0_LEFT"] = FlipTextureHorizontal(textures["SMALLTOAD_0_RIGHT"]);





        textures["SMALLTOAD_1_RIGHT"] = LoadTexture("resources/images/Object/toad/SmallToad_1.png");


        textures["SMALLTOAD_1_LEFT"] = FlipTextureHorizontal(textures["SMALLTOAD_1_RIGHT"]);





        textures["SMALLTOAD_JUMPING_RIGHT"] = LoadTexture("resources/images/Object/toad/SmallToadJumping_0.png");


        textures["SMALLTOAD_JUMPING_LEFT"] = FlipTextureHorizontal(textures["SMALLTOAD_JUMPING_RIGHT"]);


        textures["SMALLTOAD_DUCKING_RIGHT"] = LoadTexture("resources/images/Object/toad/SmallToadDucking_0.png");


        textures["SMALLTOAD_DUCKING_LEFT"] = FlipTextureHorizontal(textures["SMALLTOAD_DUCKING_RIGHT"]);


        textures["SMALLTOAD_FALLING_RIGHT"] = LoadTexture("resources/images/Object/toad/SmallToadFalling_0.png");


        textures["SMALLTOAD_FALLING_LEFT"] = FlipTextureHorizontal(textures["SMALLTOAD_FALLING_RIGHT"]);


        textures["TOAD_DIE"] = LoadTexture("resources/images/Object/toad/SmallToadDying_0.png");


        textures["SMALLTOAD_VICTORY"] = LoadTexture("resources/images/Object/toad/SmallToadVictory_0.png");





        // Super Toad textures


        textures["SUPER_TOAD_0_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToad_0.png");


        textures["SUPER_TOAD_1_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToad_1.png");


        textures["SUPER_TOAD_2_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToad_2.png");


        textures["SUPER_TOAD_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_0_RIGHT"]);


        textures["SUPER_TOAD_1_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_1_RIGHT"]);


        textures["SUPER_TOAD_2_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_2_RIGHT"]);





        textures["SUPER_TOAD_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToadJumping_0.png");


        textures["SUPER_TOAD_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_JUMPING_0_RIGHT"]);


        textures["SUPER_TOAD_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToadFalling_0.png");


        textures["SUPER_TOAD_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_FALLING_0_RIGHT"]);





        textures["SUPER_TOAD_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/SuperToadDucking_0.png");


        textures["SUPER_TOAD_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["SUPER_TOAD_DUCKING_0_RIGHT"]);





        textures["SUPER_TOAD_VICTORY"] = LoadTexture("resources/images/Object/toad/SuperToadVictory_0.png");





        // Transitioning Toad textures


        textures["TRANSITIONING_TOAD_0_RIGHT"] = LoadTexture("resources/images/Object/toad/TransitioningToad_0.png");


        textures["TRANSITIONING_TOAD_0_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_TOAD_0_RIGHT"]);


        textures["TRANSITIONING_TOAD_1_RIGHT"] = LoadTexture("resources/images/Object/toad/TransitioningToad_1.png");


        textures["TRANSITIONING_TOAD_1_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_TOAD_1_RIGHT"]);


        textures["TRANSITIONING_TOAD_2_RIGHT"] = LoadTexture("resources/images/Object/toad/TransitioningToad_2.png");


        textures["TRANSITIONING_TOAD_2_LEFT"] = FlipTextureHorizontal(textures["TRANSITIONING_TOAD_2_RIGHT"]);





        // Fire Toad textures


        textures["FIRE_TOAD_0_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToad_0.png");


        textures["FIRE_TOAD_1_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToad_1.png");


        textures["FIRE_TOAD_2_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToad_2.png");


        textures["FIRE_TOAD_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_0_RIGHT"]);


        textures["FIRE_TOAD_1_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_1_RIGHT"]);


        textures["FIRE_TOAD_2_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_2_RIGHT"]);





        textures["FIRE_TOAD_JUMPING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToadJumping_0.png");


        textures["FIRE_TOAD_JUMPING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_JUMPING_0_RIGHT"]);


        textures["FIRE_TOAD_FALLING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToadFalling_0.png");


        textures["FIRE_TOAD_FALLING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_FALLING_0_RIGHT"]);





        textures["FIRE_TOAD_DUCKING_0_RIGHT"] = LoadTexture("resources/images/Object/toad/FireToadDucking_0.png");


        textures["FIRE_TOAD_DUCKING_0_LEFT"] = FlipTextureHorizontal(textures["FIRE_TOAD_DUCKING_0_RIGHT"]);


        textures["FIRE_TOAD_VICTORY"] = LoadTexture("resources/images/Object/toad/FireToadVictory_0.png");


    }
    //BACKGROUND
    {
    textures["MENU_BACKGROUND"] = LoadTexture("resources/Menu/Menu.png");
    
    textures["BACKGROUND_0"] = LoadTexture("resources/images/backgrounds/background1.png");
    textures["BACKGROUND_1"] = LoadTexture("resources/images/backgrounds/background2.png");
    textures["BACKGROUND_2"] = LoadTexture("resources/images/backgrounds/background3.png");
    textures["BACKGROUND_3"] = LoadTexture("resources/images/backgrounds/background4.png");
    textures["BACKGROUND_4"] = LoadTexture("resources/images/backgrounds/background5.png");
    textures["BACKGROUND_5"] = LoadTexture("resources/images/backgrounds/background6.png");
    textures["BACKGROUND_6"] = LoadTexture("resources/images/backgrounds/background7.png");
    textures["BACKGROUND_7"] = LoadTexture("resources/images/backgrounds/background8.png");
    textures["BACKGROUND_8"] = LoadTexture("resources/images/backgrounds/background9.png");
    textures["BACKGROUND_9"] = LoadTexture("resources/images/backgrounds/background10.png");
    textures["BACKGROUND_10"] = LoadTexture("resources/images/backgrounds/background11.png");

    textures["LOGO"] = LoadTexture("resources/Menu/logo.gif");

    }
    //TILES
    for (int i = 1; i <= 104; ++i) {
        std::string keyStr = "tile_" + std::to_string(i);
        textures[keyStr] = LoadTexture(("resources/images/tiles/tile_" + std::to_string(i) + ".png").c_str());
    }
    //Button
    
    textures["START_BUTTON"] = LoadTexture("resources/images/Button/START.png");
    textures["MAP_EDITOR_BUTTON"] = LoadTexture("resources/images/Button/MAP EDITOR.png");
    textures["SETTINGS_BUTTON"] = LoadTexture("resources/images/Button/SETTING.png");
    textures["BACK_BUTTON"] = LoadTexture("resources/images/Button/BACK.png");
    textures["NEW_MAP_BUTTON"] = LoadTexture("resources/images/Button/NEW MAP.png");
    textures["LOAD_MAP_BUTTON"] = LoadTexture("resources/images/Button/LOAD MAP.png");
    textures["1_PLAYER"] = LoadTexture("resources/images/Button/1 PLAYER.png");
    textures["2_PLAYERS"] = LoadTexture("resources/images/Button/2 PLAYERS.png");
    textures["MARIO_BUTTON"] = LoadTexture("resources/images/Button/MARIO_BUTTON.png");
    textures["LUIGI_BUTTON"] = LoadTexture("resources/images/Button/LUIGI_BUTTON.png");
    textures["CHOOSE PLAYER"] = LoadTexture("resources/images/Button/CHOOSE PLAYER.png");
    textures["CHOOSE CHARACTOR"] = LoadTexture("resources/images/Button/CHOOSE CHARACTOR.png");
    textures["SETTING INTERFACE"] = LoadTexture("resources/images/Button/SETTING INTERFACE.png");
    textures["MUTE ALL ON"] = LoadTexture("resources/images/Button/MUTE ALL ON.png");
    textures["MUTE ALL OFF"] = LoadTexture("resources/images/Button/MUTE ALL OFF.png");
    textures["TUTORIALS"] = LoadTexture("resources/images/Button/TUTORIALS.png");
    textures["AUDIO SETTING"] = LoadTexture("resources/images/Button/AUDIO SETTING.png");
    textures["TOAD_BUTTON"] = LoadTexture("resources/images/toad/SmallToad_0.png");
    textures["PEACH_BUTTON"] = LoadTexture("resources/images/peach/SmallPeach_0.png");




    
    
    // textures["START_BUTTON"] = LoadTexture("../resources/images/Button/StartButton.png");
    // textures["PAUSE_BUTTON"] = LoadTexture("../resources/images/Button/PauseButton.png");
    // textures["STOP_BUTTON"] = LoadTexture("../resources/images/Button/StopButton.png");
    // textures["GREEN_ARROW"] = LoadTexture("../resources/images/Button/GreenArrow.png");
    // textures["YELLOW_ARROW"] = LoadTexture("../resources/images/Button/YellowArrow.png");
    




    //item
    {
        //coin
        textures["COIN_0"] = LoadTexture("resources/images/Object/items/Coin_0.png");
        textures["COIN_1"] = LoadTexture("resources/images/Object/items/Coin_1.png");
        textures["COIN_2"] = LoadTexture("resources/images/Object/items/Coin_2.png");
        textures["COIN_3"] = LoadTexture("resources/images/Object/items/Coin_3.png");
        //Course Clear Token
        textures["COURSE_CLEAR_TOKEN"] = LoadTexture("resources/images/Object/items/CourseClearToken.png");
        //Fire Flower
        textures["FIRE FLOWER_0"] = LoadTexture("resources/images/Object/items/FireFlower_0.png");
        textures["FIRE FLOWER_1"] = LoadTexture("resources/images/Object/items/FireFlower_1.png");
        
        //mushroom
        textures["Mushroom"] = LoadTexture("resources/images/Object/items/Mushroom.png");
        //One up Mushroom
        textures["1UpMushroom"] = LoadTexture("resources/images/Object/items/1UpMushroom.png");
        //Star
        textures["star"] = LoadTexture("resources/images/Object/items/Star.png");
        //Three Up Moon
        textures["3UpMoon"] = LoadTexture("resources/images/Object/items/3UpMoon.png");
        //Yoshi Coin 
        textures["YoshiCoin_0"] = LoadTexture("resources/images/Object/items/YoshiCoin_0.png");
        textures["YoshiCoin_1"] = LoadTexture("resources/images/Object/items/YoshiCoin_1.png");
        textures["YoshiCoin_2"] = LoadTexture("resources/images/Object/items/YoshiCoin_2.png");
        textures["YoshiCoin_3"] = LoadTexture("resources/images/Object/items/YoshiCoin_3.png");
    }
    //ENEMY
    {
        textures["GOOMBA_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/Goomba_0.png");
        textures["GOOMBA_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/Goomba_1.png");
        textures["GOOMBA_0_LEFT"] = FlipTextureHorizontal(textures["GOOMBA_0_RIGHT"]);
        textures["GOOMBA_1_LEFT"] = FlipTextureHorizontal(textures["GOOMBA_1_RIGHT"]);
        //Koopa Troopa
        textures["GreenKoopaTroopa_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/GreenKoopaTroopa_0.png");
        textures["GreenKoopaTroopa_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/GreenKoopaTroopa_1.png");
        textures["GreenKoopaTroopa_0_LEFT"] = FlipTextureHorizontal(textures["GreenKoopaTroopa_0_RIGHT"]);
        textures["GreenKoopaTroopa_1_LEFT"] = FlipTextureHorizontal(textures["GreenKoopaTroopa_1_RIGHT"]);

        textures["RedKoopaTroopa_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/RedKoopaTroopa_0.png");
        textures["RedKoopaTroopa_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/RedKoopaTroopa_1.png");
        textures["RedKoopaTroopa_0_LEFT"] = FlipTextureHorizontal(textures["RedKoopaTroopa_0_RIGHT"]);
        textures["RedKoopaTroopa_1_LEFT"] = FlipTextureHorizontal(textures["RedKoopaTroopa_1_RIGHT"]);

        textures["YellowKoopaTroopa_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/YellowKoopaTroopa_0.png");
        textures["YellowKoopaTroopa_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/YellowKoopaTroopa_1.png");
        textures["YellowKoopaTroopa_0_LEFT"] = FlipTextureHorizontal(textures["YellowKoopaTroopa_0_RIGHT"]);
        textures["YellowKoopaTroopa_1_LEFT"] = FlipTextureHorizontal(textures["YellowKoopaTroopa_1_RIGHT"]);

        textures["BlueKoopaTroopa_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/BlueKoopaTroopa_0.png");
        textures["BlueKoopaTroopa_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/BlueKoopaTroopa_1.png");
        textures["BlueKoopaTroopa_0_LEFT"] = FlipTextureHorizontal(textures["BlueKoopaTroopa_0_RIGHT"]);
        textures["BlueKoopaTroopa_1_LEFT"] = FlipTextureHorizontal(textures["BlueKoopaTroopa_1_RIGHT"]);


        textures["BuzzyBeetle_0_Right"] = LoadTexture("resources/images/Object/Enemy/BuzzyBeetle_0.png");
        textures["BuzzyBeetle_1_Right"] = LoadTexture("resources/images/Object/Enemy/BuzzyBeetle_1.png");
        textures["BuzzyBeetle_0_Left"] = FlipTextureHorizontal(textures["BuzzyBeetle_0_Right"]);
        textures["BuzzyBeetle_1_Left"] = FlipTextureHorizontal(textures["BuzzyBeetle_1_Right"]);

        textures["BULLETBILL_RIGHT"] = LoadTexture("resources/images/Object/Enemy/BulletBill_0.png");
        textures["BULLETBILL_LEFT"] = FlipTextureHorizontal(textures["BULLETBILL_RIGHT"]);

        textures["Bob-omb_Right_0"]= LoadTexture("resources/images/Object/Enemy/BobOmb_0.png");
        textures["Bob-omb_Right_1"]= LoadTexture("resources/images/Object/Enemy/BobOmb_1.png");
        textures["Bob-omb_Left_0"]= FlipTextureHorizontal(textures["Bob-omb_Right_0"]);
        textures["Bob-omb_Left_1"]= FlipTextureHorizontal(textures["Bob-omb_Right_1"]);
        textures["Bob-omb_Explosion"] = LoadTexture("resources/images/Object/Enemy/BobOmbExplosion_0.png");

        textures["BANZAIBILL"]= LoadTexture("resources/images/Object/Enemy/BanzaiBill_0.png");

        textures["REX_0_RIGHT"] = LoadTexture("resources/images/Object/Enemy/Rex_2_0.png");
        textures["REX_1_RIGHT"] = LoadTexture("resources/images/Object/Enemy/Rex_2_1.png");
        textures["REX_0_LEFT"] = FlipTextureHorizontal(textures["REX_0_RIGHT"]);
        textures["REX_1_LEFT"] = FlipTextureHorizontal(textures["REX_1_RIGHT"]);

        textures["REX_3_RIGHT"]= LoadTexture("resources/images/Object/Enemy/Rex_1_0.png");
        textures["REX_4_RIGHT"]= LoadTexture("resources/images/Object/Enemy/Rex_1_1.png");
        textures["REX_3_LEFT"]= FlipTextureHorizontal(textures["REX_3_RIGHT"]);
        textures["REX_4_LEFT"]= FlipTextureHorizontal(textures["REX_4_RIGHT"]);
        //Flying Goomba
        textures["FlyingGoomba_Flap_Right"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_2.png");
        textures["FlyingGoomba_WingDown_Right"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_3.png");
        textures["FlyingGoomba_Flap_Center"] = FlipTextureHorizontal(textures["FlyingGoomba_Flap_Right"]);
        textures["FlyingGoomba_WingDown_Center"] = FlipTextureHorizontal(textures["FlyingGoomba_WingDown_Right"]);
        //piranhaplant
        textures["PiranhaPlant_MouthOpen"] = LoadTexture("resources/images/Object/Enemy/PiranhaPlant_0.png");
        textures["PiranhaPlant_MouthClosed"] = LoadTexture("resources/images/Object/Enemy/PiranhaPlant_1.png");
        //jumping piranha 
        textures["PiranhaPlant_0"] = LoadTexture("resources/images/Object/Enemy/JumpingPiranhaPlant_0.png"); 
        textures["PiranhaPlant_1"] = LoadTexture("resources/images/Object/Enemy/JumpingPiranhaPlant_1.png");
        textures["PiranhaPlant_2"] = LoadTexture("resources/images/Object/Enemy/JumpingPiranhaPlant_2.png");
        textures["PiranhaPlant_3"] = LoadTexture("resources/images/Object/Enemy/JumpingPiranhaPlant_3.png"); 
    }

    //Blocks
    {
        textures["BLOCK_QUESTION_0"] = LoadTexture("resources/images/Object/blocks/Question_0.png");
        textures["BLOCK_QUESTION_1"] = LoadTexture("resources/images/Object/blocks/Question_1.png");
        textures["BLOCK_QUESTION_2"] = LoadTexture("resources/images/Object/blocks/Question_2.png");
        textures["BLOCK_QUESTION_3"] = LoadTexture("resources/images/Object/blocks/Question_3.png");
        textures["BLOCK_STONE"] = LoadTexture("resources/images/Object/blocks/Stone_0.png");
        textures["BLOCK_CLOUD"] = LoadTexture("resources/images/Object/blocks/Cloud_0.png");
        textures["BLOCK_WOOD"] = LoadTexture("resources/images/Object/blocks/Wood_0.png");
        textures["BLOCK_MESSAGE"] = LoadTexture("resources/images/Object/blocks/Message_0.png");
        textures["BLOCK_GLASS"] = LoadTexture("resources/images/Object/blocks/Glass_0.png");
        textures["BLOCK_GLASS_1"] = LoadTexture("resources/images/Object/blocks/Glass_1.png");
        textures["BLOCK_GLASS_2"] = LoadTexture("resources/images/Object/blocks/Glass_2.png");
        textures["BLOCK_EXCLAMATION"] = LoadTexture("resources/images/Object/blocks/Exclamation_0.png");
        textures["BLOCK_EYE_CLOSED"] = LoadTexture("resources/images/Object/blocks/EyesClosed_0.png");
        textures["BLOCK_EYE_OPEN_0"] = LoadTexture("resources/images/Object/blocks/EyesOpened_0.png");
        textures["BLOCK_EYE_OPEN_1"] = LoadTexture("resources/images/Object/blocks/EyesOpened_1.png");
        textures["BLOCK_EYE_OPEN_2"] = LoadTexture("resources/images/Object/blocks/EyesOpened_2.png");
        textures["BLOCK_EYE_OPEN_3"] = LoadTexture("resources/images/Object/blocks/EyesOpened_3.png");
        textures["BASE"] = LoadTexture("resources/images/Object/blocks/base/tiles.png");
    }

    //animation of item


    {
        textures["+200"] = LoadTexture("resources/images/gui/guiHundredsPoints_200.png");
        textures["+2000"] = LoadTexture("resources/images/gui/guiThousandsPoints_2000.png");
        textures["+1000"] = LoadTexture("resources/images/gui/guiThousandsPoints_1000.png");
        textures["+1UP"] = LoadTexture("resources/images/gui/gui1Up.png");
        textures["+3UP"] = LoadTexture("resources/images/gui/gui3Up.png");

    }
    // animation effect
    {
        textures["SMOKE_0"] = LoadTexture("resources/images/Object/effects/Puft_0.png");
        textures["SMOKE_1"] = LoadTexture("resources/images/Object/effects/Puft_1.png");
        textures["SMOKE_2"] = LoadTexture("resources/images/Object/effects/Puft_2.png");
        textures["SMOKE_3"] = LoadTexture("resources/images/Object/effects/Puft_3.png");

    }
    // GUI
    {
        textures["GUI_COIN"] = LoadTexture("resources/images/gui/guiCoin.png");
        textures["GUI_MARIO"] = LoadTexture("resources/images/gui/guiMario.png");
        textures["GUI_BLOCK"] = LoadTexture("resources/images/gui/guiNextItem.png");
        textures["FIREFLOWER_ICON"] = LoadTexture("resources/images/Object/items/FireFlower_0.png");
        textures["MUSHROOM_ICON"] = LoadTexture("resources/images/Object/items/Mushroom.png");
        textures["GAME_OVER"] = LoadTexture("resources/images/gui/guiGameOver.png");
    }
    {
        textures["Moving 1"]=LoadTexture("resources/images/Object/Enemy/Moving 1.png");
        textures["Moving 2"]=LoadTexture("resources/images/Object/Enemy/Moving 2.png");
        textures["Moving 3"]=LoadTexture("resources/images/Object/Enemy/Moving 3.png");
        textures["Moving 4"]=LoadTexture("resources/images/Object/Enemy/Moving 4.png");

        textures["Skill 1_1"] = LoadTexture("resources/images/Object/Enemy/Skill 1-1.png");
        textures["Skill 1_2"] = LoadTexture("resources/images/Object/Enemy/Skill 1-2.png");
        textures["Skill 1_3"] = LoadTexture("resources/images/Object/Enemy/Skill 1-3.png");
        textures["Skill 1_4"] = LoadTexture("resources/images/Object/Enemy/Skill1-4.png");
        textures["Skill 1_5,7"] = LoadTexture("resources/images/Object/Enemy/Skill 1-5,7.png");
        textures["Skill 1_6"] = LoadTexture("resources/images/Object/Enemy/Skill 1-6.png");

        textures["Skill 2_1"] = LoadTexture("resources/images/Object/Enemy/Skill 2-1.png");
        textures["Skill 2_2"] = LoadTexture("resources/images/Object/Enemy/Skill 2-2.png");
        textures["Skill 2_3"] = LoadTexture("resources/images/Object/Enemy/Skill 2-3.png");
        textures["Skill 2_4"] = LoadTexture("resources/images/Object/Enemy/Skill 2-4.png");

        textures["Skill 3_1"] = LoadTexture("resources/images/Object/Enemy/Skill 3-1.png");
        textures["Skill 3_2"] = LoadTexture("resources/images/Object/Enemy/Skill 3-2.png");
        textures["Skill 3_3,6"] = LoadTexture("resources/images/Object/Enemy/Skill 3-3,6.png");
        textures["Skill 3_4"] = LoadTexture("resources/images/Object/Enemy/Skill 3-4.png");
        textures["Skill 3_5"] = LoadTexture("resources/images/Object/Enemy/Skill3-5.png");

        textures["Skill 4_1"] = LoadTexture("resources/images/Object/Enemy/Skill 4-1.png");
        textures["Skill 4_2"] = LoadTexture("resources/images/Object/Enemy/Skill 4-2.png");
        textures["Skill 4_3"] = LoadTexture("resources/images/Object/Enemy/Skill 4-3.png");

        textures["Waiting"] = LoadTexture("resources/images/Object/Enemy/Waiting Boss.png");
        textures["Skill 6_1"] = LoadTexture("resources/images/Object/Enemy/Skill 6-1.png");
        textures["Skill 6_2"] = LoadTexture("resources/images/Object/Enemy/Skill 6-2.png");
        textures["Skill 6_3"] = LoadTexture("resources/images/Object/Enemy/Skill 6-3.png");
        textures["Skill 6_4"] = LoadTexture("resources/images/Object/Enemy/Skill 6-4.png");
        textures["Skill 6_5"] = LoadTexture("resources/images/Object/Enemy/Skill 6-5.png");
        textures["Skill 6_6"] = LoadTexture("resources/images/Object/Enemy/Skill 6-6.png");
        textures["Skill 6_8"] = LoadTexture("resources/images/Object/Enemy/Skill 6-8.png");
        // textures["Skill 6_7"] = LoadTexture("resources/images/gui/Skill 6-7.png");
        textures["Skill 6_9"] = LoadTexture("resources/images/Object/Enemy/Skill 6-9.png");
        textures["Skill 6_10"] = LoadTexture("resources/images/Object/Enemy/Skill 6-10.png");
        
        textures["Fire_Small_UP_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 1.png");
        textures["Fire_Normal_UP_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 2.png");
        textures["Fire_Big_UP_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 3.png");
        textures["Fire_Small_UP_Right"]= FlipTextureHorizontal(textures["Fire_Small_UP_Left"]);
        textures["Fire_Normal_UP_Right"]= FlipTextureHorizontal(textures["Fire_Normal_UP_Left"]);
        textures["Fire_Big_UP_Right"]= FlipTextureHorizontal(textures["Fire_Big_UP_Left"]);
        textures["Fire_Small_STRAIGHT_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 4.png");
        textures["Fire_Normal_STRAIGHT_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 5.png");
        textures["Fire_Big_STRAIGHT_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 6.png");
        textures["Fire_Small_STRAIGHT_Right"]= FlipTextureHorizontal(textures["Fire_Small_STRAIGHT_Left"]);
        textures["Fire_Normal_STRAIGHT_Right"]= FlipTextureHorizontal(textures["Fire_Normal_STRAIGHT_Left"]);
        textures["Fire_Big_STRAIGHT_Right"]= FlipTextureHorizontal(textures["Fire_Big_STRAIGHT_Left"]);
        textures["Fire_Small_DOWN_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 7.png");
        textures["Fire_Normal_DOWN_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 8.png");
        textures["Fire_Big_DOWN_Left"]= LoadTexture("resources/images/Object/Enemy/Fire 9.png");
        textures["Fire_Small_DOWN_Right"]= FlipTextureHorizontal(textures["Fire_Small_DOWN_Left"]);
        textures["Fire_Normal_DOWN_Right"]= FlipTextureHorizontal(textures["Fire_Normal_DOWN_Left"]);
        textures["Fire_Big_DOWN_Right"]= FlipTextureHorizontal(textures["Fire_Big_DOWN_Left"]);
    }
    {
        textures["HealthBar1"] = LoadTexture("resources/images/Object/HealthBar/Full.png");
        textures["HealthBar2"] = LoadTexture("resources/images/Object/HealthBar/Full2.png");
        textures["HealthBar3"] = LoadTexture("resources/images/Object/HealthBar/Full3.png");
        textures["HealthBar4"] = LoadTexture("resources/images/Object/HealthBar/Full4.png");
        textures["HealthBar5"] = LoadTexture("resources/images/Object/HealthBar/Full5.png");
    }
}

void ResrcManager::loadSounds(){
    sounds["MARIO_JUMP"] = LoadSound("resources/SFX/smw_jump.wav");
    sounds["MARIO_FIREBALL"] = LoadSound("resources/SFX/smw_fireball.wav");

    sounds["MARIO_POWERUP"] = LoadSound("resources/sfx/smw_power-up.wav");
    sounds["MARIO_BEING_HIT"] = LoadSound("resources/sfx/smw_pipe.wav");
    sounds["MARIO_DEATH"] = LoadSound("resources/musics/playerDown.mp3");
    sounds["COIN_COLLECTION"] = LoadSound("resources/sfx/smw_coin.wav");
    sounds["ENEMY_DEATH"] = LoadSound("resources/sfx/smw_stomp.wav");
    sounds["POWER_UP_APPEARS"] = LoadSound("resources/sfx/smw_power-up_appears.wav");

}

void ResrcManager::loadMusics(){
    musics["GAME_OVER"] = LoadMusicStream("resources/musics/gameOver.mp3");
    musics["GAMEWORLD_1"] = LoadMusicStream("resources/musics/music1.mp3");
    musics["GAMEWORLD_2"] = LoadMusicStream("resources/musics/music5.mp3");
    musics["GAMEWORLD_3"] = LoadMusicStream("resources/musics/music3.mp3");
    musics["MENU"] = LoadMusicStream("resources/musics/title.mp3");
    musics["VICTORY"] = LoadMusicStream("resources/musics/courseClear.mp3");

}

void ResrcManager::loadFonts() {
    
    fonts["SUPER_MARIO_WORLD_FONT"] = LoadFont("resources/Font/SuperMarioWorld.ttf");
}

void ResrcManager::unloadFonts() {
    for (auto &pair : fonts) {
        UnloadFont(pair.second);
    }
    fonts.clear();
}

void ResrcManager::loadResources() {
    loadFonts();
    loadTextures();
    loadSounds();
    loadMusics();
}

void ResrcManager::unloadTextures() {
    for (auto &pair : textures) {
        UnloadTexture(pair.second);
    }
    textures.clear();
}

void ResrcManager::unloadSounds() {
    for (auto &pair : sounds) {
        UnloadSound(pair.second);
    }
    sounds.clear();
}

void ResrcManager::unloadMusics() {
    for (auto &pair : musics) {
        UnloadMusicStream(pair.second);
    }
    musics.clear();
}

ResrcManager &ResrcManager::GetInstance() {
    if (instance == nullptr) {
        instance = new ResrcManager();
    }
    return *instance;
}

Texture2D& ResrcManager::getTexture(const std::string &name) {
    return textures[name];
}

Sound& ResrcManager::getSound(const std::string &name) {
    return sounds[name];
}

Music& ResrcManager::getMusic(const std::string &name) {
    return musics[name];
}

Font& ResrcManager::getFont(const std::string &name)
{
    return fonts[name];
}



void ResrcManager::unloadResources() {
    unloadTextures();
    unloadSounds();
    unloadMusics();
    unloadFonts();
}

Texture2D FlipTextureHorizontal(const Texture2D &texture) {
    Image image = LoadImageFromTexture(texture); // Load image from texture
    ImageFlipHorizontal(&image); // Flip the image horizontally
    Texture2D flippedTexture = LoadTextureFromImage(image); // Create a new texture from the flipped image
    UnloadImage(image); // Unload the image to free memory
    return flippedTexture;
}