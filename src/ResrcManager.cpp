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
        textures["MARIO_DIE"] = LoadTexture("resources/images/Object/mario/SmallMario_0.png");
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
            textures["LUIGI_DIE"] = LoadTexture("resources/images/Object/luigi/SmallLuigi_0.png");
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
    
    }
    //TILES
    for (int i = 0; i < 104; ++i) {
        std::string keyStr = "tile_" + std::to_string(i);
        textures[keyStr] = LoadTexture(("resources/images/tiles/tile_" + std::to_string(i) + ".png").c_str());
    }
    //Button
<<<<<<< HEAD
    // textures["START_BUTTON"] = LoadTexture("resources/Menu/StartButton.png");
=======
    textures["START_BUTTON"] = LoadTexture("resources/Menu/StartButton.png");
    textures["MAP_EDITOR_BUTTON"] = LoadTexture("resources/Menu/MapEditorButton.png");
    textures["BACK_BUTTON"] = LoadTexture("resources/Menu/BackButton.png");
    textures["NEW_MAP_BUTTON"] = LoadTexture("resources/Menu/NewMapButton.png");
    textures["LOAD_MAP_BUTTON"] = LoadTexture("resources/Menu/LoadMapButton.png");
>>>>>>> origin/mapedit
    // textures["EXIT_BUTTON"] = LoadTexture("resources/Menu/ExitButton.png");
    // textures["SETTINGS_BUTTON"] = LoadTexture("resources/Menu/SettingsButton.png");
    textures["START_BUTTON"] = LoadTexture("../resources/images/Button/StartButton.png");
    textures["PAUSE_BUTTON"] = LoadTexture("../resources/images/Button/PauseButton.png");
    textures["STOP_BUTTON"] = LoadTexture("../resources/images/Button/StopButton.png");
    textures["GREEN_ARROW"] = LoadTexture("../resources/images/Button/GreenArrow.png");
    textures["YELLOW_ARROW"] = LoadTexture("../resources/images/Button/YellowArrow.png");



    //item
    {
        //coin
        textures["COIN_0"] = LoadTexture("resources/images/Object/items/Coin_0.png");
        textures["COIN_1"] = LoadTexture("resources/images/Object/items/Coin_1.png");
        textures["COIN_2"] = LoadTexture("resources/images/Object/items/Coin_2.png");
        textures["COIN_3"] = LoadTexture("resources/images/Object/items/Coin_3.png");
        //Course Clear Token
        textures["COURSE CLEAR TOKEN"] = LoadTexture("resources/images/Object/items/CourseClearToken.png");
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

        textures["BulletBill"] = LoadTexture("resources/images/Object/Enemy/BulletBill_0.png");

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
        textures["FlyingGoomba_Flap_Center"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_0.png");
        textures["FlyingGoomba_WingDown_Center"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_1.png");
        textures["FlyingGoomba_Flap_Right"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_2.png");
        textures["FlyingGoomba_WingDown_Right"] = LoadTexture("resources/images/Object/Enemy/FlyingGoomba_3.png");
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
    // GUI
    {
        textures["GUI_COIN"] = LoadTexture("resources/images/gui/guiCoin.png");
        textures["GUI_MARIO"] = LoadTexture("resources/images/gui/guiMario.png");
        textures["GUI_BLOCK"] = LoadTexture("resources/images/gui/guiNextItem.png");
        textures["FIREFLOWER_ICON"] = LoadTexture("resources/images/Object/items/FireFlower_0.png");
        textures["MUSHROOM_ICON"] = LoadTexture("resources/images/Object/items/Mushroom.png");
    }
}

void ResrcManager::loadSounds(){
    sounds["MARIO_JUMP"] = LoadSound("resources/SFX/smw_jump.wav");
    sounds["MARIO_FIREBALL"] = LoadSound("resources/SFX/smw_fireball.wav");
    sounds["MARIO_POWERUP"] = LoadSound("resources/SFX/smw_power-up_appears.wav");

    sounds["MARIO_POWERUP"] = LoadSound("resources/sfx/smw_power-up.wav");
    sounds["MARIO_BEING_HIT"] = LoadSound("resources/sfx/smw_pipe.wav");
    sounds["MARIO_DEATH"] = LoadSound("resources/musics/playerDown.wav");
    sounds["COIN_COLLECTION"] = LoadSound("resources/sfx/smw_coin.wav");
    sounds["POWER_UP_APPEARS"] = LoadSound("resources/sfx/smw_power-up_appears.wav");
    sounds["POWER_UP"] = LoadSound("resources/sfx/smw_power-up.wav");

}

void ResrcManager::loadMusics(){
    musics["GAME_OVER"] = LoadMusicStream("resources/musics/gameOver.wav");
    musics["GAMEWORLD_0"] = LoadMusicStream("resources/musics/music1.wav");
    musics["GAMEWORLD_1"] = LoadMusicStream("resources/musics/music2.wav");
    musics["GAMEWORLD_2"] = LoadMusicStream("resources/musics/music3.wav");
    musics["MENU"] = LoadMusicStream("resources/musics/title.wav");
    musics["VICTORY"] = LoadMusicStream("resources/musics/courseClear.wav");

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






