#include "ResrcManager.h" 

ResrcManager* ResrcManager::instance = nullptr;

ResrcManager::~ResrcManager() {
    // Clean up resources if necessary
    unloadResources();
    
    instance = nullptr;
}

void ResrcManager::loadTextures() {
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
    textures["FIRE_MARIO_VICTORY"] = LoadTexture("resources/images/Object/mario/FireMarioVictory_0.png");

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
    textures["START_BUTTON"] = LoadTexture("resources/Menu/StartButton.png");
    // textures["EXIT_BUTTON"] = LoadTexture("resources/Menu/ExitButton.png");
    // textures["SETTINGS_BUTTON"] = LoadTexture("resources/Menu/SettingsButton.png");

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






