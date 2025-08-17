#include "../inc/World/GameWorld.h"
    #include "../inc/Item/Mushroom.h"
    #include "../inc/Item/OneUpMushroom.h"
    #include "../inc/Item/Star.h"
    #include "../inc/Item/ThreeUpMoon.h"
    #include "../inc/Character/Luigi.h"
    #include "../inc/Item/YoshiCoin.h"
    #include "../inc/Enemy/EnemyManager.h"
    #include "../inc/World/MediatorCollision.h"
    #include "../inc/Character/Mario.h"
    #include "../inc/Block/CloudBlock.h"
    #include "../inc/Block/EyesClosedBlock.h"
    #include "../inc/Block/EyesOpenedBlock.h"
    #include "../inc/Block/GlassBlock.h"
    #include "../inc/Block/QuestionBlock.h"
    #include "../inc/Block/WoodBlock.h"
    #include "../inc/Character/Toad.h"
    #include "../inc/Character/Peach.h"
    #include "Boss.h"
    #include "../inc/Item/ItemType.h"
    #include "../inc/Block/BlockType.h"
    #include "../inc/Enemy/EnemyType.h"
    GameWorld::GameWorld() : player1(nullptr), player2(nullptr), interactiveTiles(map.getInteractiveTiles())
{
    player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    map.LoadMap(5);
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");
}

GameWorld::GameWorld(int MapID, GameScreen *gameScreen, bool multiplayer,
                     CharacterType p1Type, CharacterType p2Type) : 
                     player1(nullptr), player2(nullptr),
                     interactiveTiles(map.getInteractiveTiles()),
                     gameScreen(gameScreen),
                     gameState(GameState::GAME_PLAYING),
                     isMultiplayer(multiplayer),
                     player1Character(p1Type),
                     player2Character(p2Type),
                     selectedMapId(MapID),
                     enemySpeedMultiplier(1.0f)
{
    map.LoadMap(MapID);
    
    // Initialize Player 1
    switch(p1Type) {
        case CharacterType::MARIO:
            player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
            break;
        case CharacterType::LUIGI:
            player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
            break;
        case CharacterType::TOAD:
            player1 = new Toad(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
            break;
        case CharacterType::PEACH:
            player1 = new Peach(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
            break;
    }
    
    // Initialize Player 2 if multiplayer
    if (multiplayer) {
        if (p2Type == CharacterType::MARIO) {
            player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
        } else {
            player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
        }
    }
    
    // Initialize Boss for MapID == 1
    if (MapID == 1) {
        // Boss* boss = new Boss(Vector2{1200, 535}, player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
        // map.GetEnemies().push_back(boss);
        // map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
    }
    
    
    // Set background based on MapID
    switch (MapID)
    {
        case 0: background = ResrcManager::GetInstance().getTexture("BACKGROUND_0"); break;
        case 1: background = ResrcManager::GetInstance().getTexture("BACKGROUND_1"); break;
        case 2: background = ResrcManager::GetInstance().getTexture("BACKGROUND_2"); break;
        case 3: background = ResrcManager::GetInstance().getTexture("BACKGROUND_3"); break;
        case 4: background = ResrcManager::GetInstance().getTexture("BACKGROUND_4"); break;
        case 5: background = ResrcManager::GetInstance().getTexture("BACKGROUND_5"); break;
        case 6: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 7: background = ResrcManager::GetInstance().getTexture("BACKGROUND_7"); break;
        case 8: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 9: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
    }
    
    // Add items for MapID == 1
    if (MapID == 1) {
        // map.GetInteractiveItems().push_back(std::make_shared<Coin>(Vector2{400, 800}));
        // map.GetInteractiveItems().push_back(std::make_shared<FireFlower>(Vector2{450, 800}));
        // map.GetInteractiveItems().push_back(std::make_shared<Star>(Vector2{600, 500}));
        // map.GetInteractiveItems().push_back(std::make_shared<YoshiCoin>(Vector2{700, 800}));
        // // map.GetInteractiveItems().push_back(std::make_shared<CourseClearToken>(Vector2{800, 800}));
        // map.GetInteractiveItems().push_back(std::make_shared<Mushroom>(Vector2{200, 800}));
        // map.GetInteractiveItems().push_back(std::make_shared<ThreeUpMoon>(Vector2{200, 500}));
        // map.GetInteractiveItems().push_back(std::make_shared<OneUpMushroom>(Vector2{200, 800}));

        // map.GetEnemies().push_back(new BanzaiBill(Vector2{500, 800}));
        // map.GetEnemies().push_back(new BulletBill(Vector2{600, 800}));
        // map.GetEnemies().push_back(new BuzzyBeetle(Vector2{700, 800}));
        // map.GetEnemies().push_back(new FlyingGoomba(Vector2{800, 800}));
        // map.GetEnemies().push_back(new Goomba(Vector2{900, 800}));
        // map.GetEnemies().push_back(new GreenKoopa(Vector2{1000, 800}));
        // map.GetEnemies().push_back(new JumpingPiranhaPlant(Vector2{1100, 800}));
        // map.GetEnemies().push_back(new RedKoopa(Vector2{1200, 800}));
        // map.GetEnemies().push_back(new Rex(Vector2{1300, 800}));
        // map.GetEnemies().push_back(new YellowKoopa(Vector2{1400, 800}));

        // for (int i = 0; i < 20; i++) {
        //     map.getBlocks().push_back(new WoodBlock(Vector2{0, float(600 + i * 32)}, Vector2{32, 32}, WHITE));
        // }
        
        // for (int i = 0; i < 20; i++) {
        //     map.getBlocks().push_back(new WoodBlock(Vector2{1920, float(600 + i * 32)}, Vector2{32, 32}, WHITE));
        // }

        // map.getBlocks().push_back(new CloudBlock(Vector2{100, 750}, Vector2{32, 32}, WHITE));
        // map.getBlocks().push_back(new EyesClosedBlock(Vector2{150, 750}, Vector2{32, 32}, WHITE));
        // map.getBlocks().push_back(new EyesOpenedBlock(Vector2{200, 750}, Vector2{32, 32}, WHITE));
        // map.getBlocks().push_back(new GlassBlock(Vector2{250, 750}, Vector2{32, 32}, WHITE));
        // map.getBlocks().push_back(new QuestionBlock(Vector2{300, 750}, Vector2{32, 32}, WHITE, GIFT_COIN));
        // map.getBlocks().push_back(new WoodBlock(Vector2{350, 750}, Vector2{32, 32}, WHITE));



    }
    
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

GameWorld::GameWorld(int MapID, GameScreen* gameScreen, bool multiplayer,
                     CharacterType p1Type, CharacterType p2Type, float speedMultiplier) : 
                     player1(nullptr), player2(nullptr),
                     interactiveTiles(map.getInteractiveTiles()),
                     gameScreen(gameScreen),
                     gameState(GameState::GAME_PLAYING),
                     isMultiplayer(multiplayer),
                     player1Character(p1Type),
                     player2Character(p2Type),
                     selectedMapId(MapID),
                     enemySpeedMultiplier(speedMultiplier)
{
    map.LoadMap(MapID);
    
    // Initialize Player 1
    if (p1Type == CharacterType::MARIO) {
        player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
    } else {
        player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::WASD);
    }
    
    // Initialize Player 2 if multiplayer
    if (multiplayer) {
        if (p2Type == CharacterType::MARIO) {
            player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
        } else {
            player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
        }
    }
    
    // Initialize Boss for MapID == 1
    if (MapID == 1) {
        Boss* boss = new Boss(Vector2{1200, 535}, player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
        map.GetEnemies().push_back(boss);
        map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
    }
    
    // Set background based on MapID
    switch (MapID)
    {
        case 0: background = ResrcManager::GetInstance().getTexture("BACKGROUND_0"); break;
        case 1: background = ResrcManager::GetInstance().getTexture("BACKGROUND_1"); break;
        case 2: background = ResrcManager::GetInstance().getTexture("BACKGROUND_2"); break;
        case 3: background = ResrcManager::GetInstance().getTexture("BACKGROUND_3"); break;
        case 4: background = ResrcManager::GetInstance().getTexture("BACKGROUND_4"); break;
        case 5: background = ResrcManager::GetInstance().getTexture("BACKGROUND_5"); break;
        case 6: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 7: background = ResrcManager::GetInstance().getTexture("BACKGROUND_7"); break;
        case 8: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 9: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
    }
    
    // Add items for MapID == 1
    if (MapID == 1) {
        map.GetInteractiveItems().push_back(std::make_shared<Coin>(Vector2{400, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<FireFlower>(Vector2{450, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<Star>(Vector2{600, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<YoshiCoin>(Vector2{700, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<Mushroom>(Vector2{200, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<ThreeUpMoon>(Vector2{200, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<OneUpMushroom>(Vector2{200, 800}));

        // Create enemies with speed multiplier applied
        BanzaiBill* banzai = new BanzaiBill(Vector2{500, 800});
        banzai->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(banzai);
        
        BulletBill* bullet = new BulletBill(Vector2{600, 800});
        bullet->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(bullet);
        
        BuzzyBeetle* buzzy = new BuzzyBeetle(Vector2{700, 800});
        buzzy->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(buzzy);
        
        FlyingGoomba* flyingGoomba = new FlyingGoomba(Vector2{800, 800});
        flyingGoomba->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(flyingGoomba);
        
        Goomba* goomba = new Goomba(Vector2{900, 800});
        goomba->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(goomba);
        
        GreenKoopa* greenKoopa = new GreenKoopa(Vector2{1000, 800});
        greenKoopa->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(greenKoopa);
        
        JumpingPiranhaPlant* piranha = new JumpingPiranhaPlant(Vector2{1100, 800});
        piranha->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(piranha);
        
        RedKoopa* redKoopa = new RedKoopa(Vector2{1200, 800});
        redKoopa->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(redKoopa);
        
        Rex* rex = new Rex(Vector2{1300, 800});
        rex->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(rex);
        
        YellowKoopa* yellowKoopa = new YellowKoopa(Vector2{1400, 800});
        yellowKoopa->SetSpeedMultiplier(speedMultiplier);
        map.GetEnemies().push_back(yellowKoopa);

        for (int i = 0; i < 20; i++) {
            map.getBlocks().push_back(new WoodBlock(Vector2{0, float(600 + i * 32)}, Vector2{32, 32}, WHITE));
        }
        
        for (int i = 0; i < 20; i++) {
            map.getBlocks().push_back(new WoodBlock(Vector2{1920, float(600 + i * 32)}, Vector2{32, 32}, WHITE));
        }

        map.getBlocks().push_back(new CloudBlock(Vector2{100, 750}, Vector2{32, 32}, WHITE));
        map.getBlocks().push_back(new EyesClosedBlock(Vector2{150, 750}, Vector2{32, 32}, WHITE));
        map.getBlocks().push_back(new EyesOpenedBlock(Vector2{200, 750}, Vector2{32, 32}, WHITE));
        map.getBlocks().push_back(new GlassBlock(Vector2{250, 750}, Vector2{32, 32}, WHITE));
        map.getBlocks().push_back(new QuestionBlock(Vector2{300, 750}, Vector2{32, 32}, WHITE, GIFT_COIN));
        map.getBlocks().push_back(new WoodBlock(Vector2{350, 750}, Vector2{32, 32}, WHITE));
    }
    
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

GameWorld::GameWorld(int level, GameScreen* gameScreen) 
    : gameState(GameState::GAME_PLAYING),
      gameScreen(gameScreen),
      isMultiplayer(false),
      player1Character(CharacterType::MARIO),
      player2Character(CharacterType::LUIGI),
      selectedMapId(level),
      enemySpeedMultiplier(1.0f),
      player1(nullptr),
      player2(nullptr),
      interactiveTiles(map.getInteractiveTiles())
{
    map.LoadMap(level);
    
    player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    player2 = nullptr;
    
    map.SetMarioPositionForBosses(player1->GetPosPtr(), nullptr, false);
    
    camera.target = Vector2{player1->GetPos().x, player1->GetPos().y};
    camera.offset = Vector2{400, 300};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

GameWorld::~GameWorld()
{
    if (player1) {
        delete player1;
        player1 = nullptr;
    }
    if (player2) {
        delete player2;
        player2 = nullptr;
    }
}



void GameWorld::UpdateWorld()
{
    // Update Player 1
    if (player1)
    {
        player1->UpdateStateAndPhysic();
        player1->UpdateCollisionProbes();
        
        // Set current player cho ScoreEffect system
        ScoreEffect::SetCurrentPlayer(player1);
        if (player1->GetState() != OBJECT_STATE_DEAD &&
            player1->GetState() != OBJECT_STATE_DYING &&
            player1->GetState() != OBJECT_STATE_VICTORY)
        {
            if (player1->GetPos().y > 900) // KIỂM TRA VỊ TRÍ PLAYER 1
            {
                player1->Die();
            }
            
            // Handle Player 1 collisions with tiles
            for (auto const &tile : interactiveTiles)
            {
                if (player1->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, tile);
            }

            // Handle Player 1 collisions with items
            for (auto const &item : map.GetInteractiveItems())
            {
                if (item && player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, item.get());
            }

            // Handle Player 1 collisions with enemies
            for (Enemy *enemy : map.GetEnemies())
            {
                if (enemy && enemy->GetState() != OBJECT_STATE_DEAD &&
                    enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                    player1->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                {
                    if (Boss *boss = dynamic_cast<Boss *>(enemy))
                    {
                        if (boss->IsDead())
                            continue; // Bỏ qua nếu boss đã chết
                    }
                    mediatorCollision.HandleCollision(player1, enemy);
                }
                // Handle Player 1 collisions with boss fireballs
                if (Boss *boss = dynamic_cast<Boss *>(enemy))
                {
                    for (BossFireball *fireball : boss->getProjectiles())
                    {
                        if (fireball && fireball->GetState() != OBJECT_STATE_DEAD &&
                            player1->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            mediatorCollision.HandleCollision(player1, fireball);
                            fireball->SetState(OBJECT_STATE_DEAD); // Mark fireball for removal
                        }
                    }
                }
            }

            // Handle Player 1 collisions with blocks
            for (auto &block : map.getBlocks())
            {
                if (block)
                {
                    CollisionType collision = block->checkCollisionType(*player1);
                    if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_EYES_OPENED)
                    {
                        if (!block->isHit())
                            player1->SetVel(Vector2{player1->GetVel().x, 0});
                        block->doHit(*player1, this->GetMap());
                    }
                    if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_QUESTION)
                    {
                        block->doHit(*player1, this->GetMap());
                        player1->SetVel({player1->GetVel().x, 0});
                    }
                    if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_GLASS)
                    {
                        block->doHit(*player1, this->GetMap());
                        player1->SetVel({player1->GetVel().x, 0});
                    }
                    mediatorCollision.HandleCollision(player1, block);
                }
            }
        }

        if (player1->GetFireballs())
        {
            for (auto &fireball : *player1->GetFireballs())
            {
                if (fireball)
                {
                    // Fireball vs tiles
                    for (auto const &tile : interactiveTiles)
                    {
                        if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(fireball, tile);
                    }
                    // Fireball vs enemies
                    for (Enemy *enemy : map.GetEnemies())
                    {
                        if (enemy && enemy->GetState() != OBJECT_STATE_DEAD &&
                            enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                            enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            // Kiểm tra boss
                            if (Boss *boss = dynamic_cast<Boss *>(enemy))
                            {
                                if (boss->IsDead())
                                    continue; // Bỏ qua nếu boss đã chết
                            }
                            mediatorCollision.HandleCollision(enemy, fireball);
                        }
                    }
                }
            }
        }
    }

    // Update Player 2 (if multiplayer)
    if (isMultiplayer && player2)
    {
        player2->UpdateStateAndPhysic();
        player2->UpdateCollisionProbes();

        if (player2->GetState() != OBJECT_STATE_DEAD &&
            player2->GetState() != OBJECT_STATE_DYING &&
            player2->GetState() != OBJECT_STATE_VICTORY)
        {
            // if (player2->GetPos().x > 1900)
            // {
            //     player2->SetState(OBJECT_STATE_VICTORY);
            //     player1->SetState(OBJECT_STATE_VICTORY);
            //     player1->Victory();
            //     player2->Victory(); // Ensure both players reach victory state
            // }
            if (player2->GetPos().y > 900) // KIỂM TRA VỊ TRÍ PLAYER 2
            {
                player2->Die();
            }
            // Handle Player 2 collisions
            // Handle Player 2 collisions with tiles
            for (auto const &tile : interactiveTiles)
            {
                if (player2->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, tile);
            }

            // Handle Player 2 collisions with items
            for (auto const &item : map.GetInteractiveItems())
            {
                if (item && player2->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, item.get());
            }

            // Handle Player 2 collisions with enemies
            for (Enemy *enemy : map.GetEnemies())
            {
                if (enemy && enemy->GetState() != OBJECT_STATE_DEAD &&
                    enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                    player2->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                {
                    if (Boss *boss = dynamic_cast<Boss *>(enemy))
                    {
                        if (boss->IsDead())
                            continue; // Bỏ qua nếu boss đã chết
                    }
                    mediatorCollision.HandleCollision(player2, enemy);
                }
                // Handle Player 2 collisions with boss fireballs
                if (Boss *boss = dynamic_cast<Boss *>(enemy))
                {
                    for (BossFireball *fireball : boss->getProjectiles())
                    {
                        if (fireball && fireball->GetState() != OBJECT_STATE_DEAD &&
                            player2->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            mediatorCollision.HandleCollision(player2, fireball);
                            fireball->SetState(OBJECT_STATE_DEAD); // Mark fireball for removal
                        }
                    }
                }
            }

            // Handle Player 2 collisions with blocks
            for (auto &block : map.getBlocks())
            {
                if (block)
                {
                    CollisionType collision2 = block->checkCollisionType(*player2);
                    if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_EYES_OPENED)
                    {
                        if (!block->isHit())
                            player2->SetVel(Vector2{player2->GetVel().x, 0});
                        block->doHit(*player2, this->GetMap());
                    }
                    if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_QUESTION)
                    {
                        block->doHit(*player2, this->GetMap());
                        player2->SetVel({player2->GetVel().x, 0});
                    }
                    if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() ==  BlockType::BLOCK_GLASS)
                    {
                        block->doHit(*player2, this->GetMap());
                        player2->SetVel({player2->GetVel().x, 0});
                    }
                    mediatorCollision.HandleCollision(player2, block);
                }
            }
        }

        if (player2->GetFireballs())
        {
            for (auto &fireball : *player2->GetFireballs())
            {
                if (fireball)
                {
                    // Fireball vs tiles
                    for (auto const &tile : interactiveTiles)
                    {
                        if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(fireball, tile);
                    }
                    // Fireball vs enemies
                    for (Enemy *enemy : map.GetEnemies())
                    {
                        if (enemy && enemy->GetState() != OBJECT_STATE_DEAD &&
                            enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                            enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            // Kiểm tra boss
                            if (Boss *boss = dynamic_cast<Boss *>(enemy))
                            {
                                if (boss->IsDead())
                                    continue; // Bỏ qua nếu boss đã chết
                            }
                            mediatorCollision.HandleCollision(enemy, fireball);
                        }
                    }
                }
            }
        }
    }

    // Update Boss positions
    map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, isMultiplayer);

    // Enemy update - AFTER player collision handling
    for (Enemy *enemy : map.GetEnemies())
    {
        if (enemy)
        {
            if (Boss *boss = dynamic_cast<Boss *>(enemy))
            {
                if (boss->IsDead())
                    continue; // Bỏ qua nếu boss đã chết
            }
            // Luôn update dying state và death effect
            enemy->UpdateDyingState();
            enemy->UpdateDeathEffect();

            // Chỉ update physics khi không dying/dead
            if (enemy->GetState() != OBJECT_STATE_DYING &&
                enemy->GetState() != OBJECT_STATE_DEAD &&
                enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED)
            {
                enemy->UpdateStateAndPhysic();

                // Enemy vs tile collisions
                for (auto const &tile : interactiveTiles)
                {
                    if (tile && enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                        mediatorCollision.HandleCollision(enemy, tile);
                }
            }
        }
    }

    // Item update
    for (auto &item : map.GetInteractiveItems())
    {
        if (item)
        {
            item->Update();
            for (auto const &tile : interactiveTiles)
            {
                if (tile && item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(item.get(), tile);
            }
        }
    }

    // Handle blocks - chỉ update, collision đã xử lý ở trên
    for (auto &block : map.getBlocks())
    {
        if (block)
        {
            block->Update();
        }
    }

    

    // Cleanup blocks
    auto &blocks = map.getBlocks();
    blocks.erase(
        std::remove_if(
            blocks.begin(),
            blocks.end(),
            [](Block *block)
            {
                if (!block || block->GetState() == OBJECT_STATE_TO_BE_REMOVED)
                {
                    delete block; // Free memory
                    return true;
                }
                return false;
            }),
        blocks.end());

    // Cleanup items
    auto &interactiveItems = map.GetInteractiveItems();
    interactiveItems.erase(
        std::remove_if(
            interactiveItems.begin(),
            interactiveItems.end(),
            [](const std::shared_ptr<Item> &item)
            {
                return (!item || item->GetState() == OBJECT_STATE_TO_BE_REMOVED || item->GetState() == OBJECT_STATE_DEAD);
            }),
        interactiveItems.end());

    // Cleanup enemies - THÊM DELETE MEMORY
    map.GetEnemies().erase(
        std::remove_if(
            map.GetEnemies().begin(),
            map.GetEnemies().end(),
            [](Enemy *enemy)
            {
                if (!enemy)
                {
                    return true;
                }
                // Kiểm tra boss
                if (Boss *boss = dynamic_cast<Boss *>(enemy))
                {
                    if (boss->IsDead())
                    {
                        delete boss;
                        return true;
                    }
                }
                // Cập nhật dying state
                enemy->UpdateDyingState();
                enemy->UpdateDeathEffect();

                // Chỉ xóa khi đã ở trạng thái DEAD
                if (enemy->GetState() == OBJECT_STATE_DEAD || enemy->GetState() == OBJECT_STATE_TO_BE_REMOVED)
                {
                    delete enemy;
                    return true;
                }
                return false;
            }),
        map.GetEnemies().end());

    // Game state logic
    bool player1Dead = (player1 && player1->GetState() == OBJECT_STATE_DEAD);
    bool player2Dead = (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_DEAD);

    if (isMultiplayer)
    {
        if (player1Dead && player2Dead)
        {
            if ((player1 && player1->GetLives() > 0) || (player2 && player2->GetLives() > 0))
            {
                gameState = GameState::GAME_RESET;
            }
            else
            {
                gameState = GameState::GAME_OVER;
            }
        }
    }
    else
    {
        if (player1Dead)
        {
            if (player1 && player1->GetLives() > 1)
            {
                gameState = GameState::GAME_RESET;
            }
            else
            {
                gameState = GameState::GAME_OVER;
            }
        }
    }

    if ((player1 && player1->GetState() == OBJECT_STATE_VICTORY) ||
        (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_VICTORY))
    {
        gameState = GameState::GAME_COMPLETED;
    }
}

void GameWorld::DrawWorld()
{
    if (isMultiplayer && player1 && player2)
    {
        Vector2 p1 = player1->GetPos();
        Vector2 p2 = player2->GetPos();
        float avgX = (p1.x + p2.x) / 2.0f;
        float avgY = (p1.y + p2.y) / 2.0f;
        float dx = fabsf(p1.x - p2.x);
        float dy = fabsf(p1.y - p2.y);
        float maxDist = 700.0f;
        float minZoom = 0.7f;
        float maxZoom = 1.3f;
        float dist = sqrtf(dx * dx + dy * dy);
        float zoom = maxZoom - (dist / maxDist) * (maxZoom - minZoom);
        if (zoom < minZoom)
            zoom = minZoom;
        if (zoom > maxZoom)
            zoom = maxZoom;
        camera.target = {avgX, avgY};
        camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
        camera.zoom = zoom;

        float camLeft = camera.target.x - (GetScreenWidth() / 2) / camera.zoom;
        float camRight = camera.target.x + (GetScreenWidth() / 2) / camera.zoom;
        float camTop = camera.target.y - (GetScreenHeight() / 2) / camera.zoom;
        float camBottom = camera.target.y + (GetScreenHeight() / 2) / camera.zoom;

        if (camLeft < 0)
            camera.target.x = (GetScreenWidth() / 2) / camera.zoom;
        if (camRight > map.GetWidth())
            camera.target.x = map.GetWidth() - (GetScreenWidth() / 2) / camera.zoom;
        if (camTop < 0)
            camera.target.y = (GetScreenHeight() / 2) / camera.zoom;
        if (camBottom > map.getHeight())
            camera.target.y = map.getHeight() - (GetScreenHeight() / 2) / camera.zoom;
    }
    else if (player1)
    {
        camera.target = player1->GetPos();
        camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
        camera.zoom = 1.3f;

        float camLeft = camera.target.x - (GetScreenWidth() / 2) / camera.zoom;
        float camRight = camera.target.x + (GetScreenWidth() / 2) / camera.zoom;
        float camTop = camera.target.y - (GetScreenHeight() / 2) / camera.zoom;
        float camBottom = camera.target.y + (GetScreenHeight() / 2) / camera.zoom;

        if (camLeft < 0)
            camera.target.x = (GetScreenWidth() / 2) / camera.zoom;
        if (camRight > map.GetWidth())
            camera.target.x = map.GetWidth() - (GetScreenWidth() / 2) / camera.zoom;
        if (camTop < 0)
            camera.target.y = (GetScreenHeight() / 2) / camera.zoom;
        if (camBottom > map.getHeight())
            camera.target.y = map.getHeight() - (GetScreenHeight() / 2) / camera.zoom;
    }

    if (camera.target.x - GetScreenWidth() / 2 >= BGpos)
        BGpos = BGpos + background.width * 1.3f;
    if (camera.target.x + GetScreenWidth() / 2 <= BGpos + background.width * 1.3f)
        BGpos = BGpos - background.width * 1.3f;

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{BGpos - background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos + background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    map.Draw();

    if (player1)
        player1->Draw();
    if (isMultiplayer && player2)
        player2->Draw();
    EndMode2D();
}


bool GameWorld::IsCompleted()
{
    return gameState == GameState::GAME_COMPLETED;
}

GameState GameWorld::GetGameState()
{
    return gameState;
}

const float GameWorld::GetGravity()
{
    return gravity;
}

void GameWorld::Init()
{
    ResrcManager::GetInstance().loadResources();
}

Map* GameWorld::GetMap()
{
    return &map;
}

Character* GameWorld::GetPlayer1()
{
    return player1;
}

Character* GameWorld::GetPlayer2()
{
    return player2;
}

bool GameWorld::IsMultiplayer()
{
    return isMultiplayer;
}

// void GameWorld::CollectWorldData(GameSaveData& saveData) {
//     // Thu thập enemies từ map
//     for (const auto& enemy : map.GetEnemies()) {
//         if (enemy) {
//             EnemySave enemyData;
//             // Determine enemy type
//             if (dynamic_cast<Goomba*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::GOOMBA);
//             } else if (dynamic_cast<GreenKoopa*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::GREENKOOPA);
//             } else if (dynamic_cast<RedKoopa*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::REDKOOPA);
//             } else if (dynamic_cast<Rex*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::REX);
//             } else if (dynamic_cast<YellowKoopa*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::YELLOWKOOPA);
//             } else if (dynamic_cast<BuzzyBeetle*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::BUZZYBEETLE);
//             } else if (dynamic_cast<FlyingGoomba*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::FLYINGGOOMBA);
//             } else if (dynamic_cast<BulletBill*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::BULLETBILL);
//             } else if (dynamic_cast<BanzaiBill*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::BANZAIBILL);
//             } else if (dynamic_cast<JumpingPiranhaPlant*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::JUMPINGPIRANHA);
//             } else if (dynamic_cast<PiranhaPlant*>(enemy)) {
//                 enemyData.enemyType = static_cast<int>(EnemyType::PIRANHAPLANT);
//             } else if (dynamic_cast<Boss*>(enemy)) {
//                 enemyData.enemyType = 99; // Special ID for Boss
//             }
            
//             enemyData.x = enemy->GetPos().x;
//             enemyData.y = enemy->GetPos().y;
//             enemyData.velX = enemy->GetVel().x;
//             enemyData.velY = enemy->GetVel().y;
//             enemyData.state = static_cast<int>(enemy->GetState());
//             saveData.enemies.push_back(enemyData);
//         }
//     }

//     // Thu thập items
//     for (const auto& item : map.GetInteractiveItems()) {
//         if (item) {
//             ItemSave itemData;
//             if (dynamic_cast<Coin*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::COIN);
//             } else if (dynamic_cast<FireFlower*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::FIREFLOWER);
//             } else if (dynamic_cast<Star*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::STAR);
//             } else if (dynamic_cast<YoshiCoin*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::YOSHICOIN);
//             } else if (dynamic_cast<Mushroom*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::MUSHROOM);
//             } else if (dynamic_cast<ThreeUpMoon*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::THREEUPMOON);
//             } else if (dynamic_cast<OneUpMushroom*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::ONEUPMUSHROOM);
//             } else if (dynamic_cast<CourseClearToken*>(item.get())) {
//                 itemData.itemType = static_cast<int>(ItemType::COURSECLEARTOKEN);
//             }

//             itemData.x = item->GetPos().x;
//             itemData.y = item->GetPos().y;
//             // itemData.isCollected = item->IsCollected();
//             saveData.items.push_back(itemData);
//         }
//     }

//     // Thu thập blocks
//     for (const auto& block : map.getBlocks()) {
//         if (block) {
//             BlockSave blockData;
//             if (dynamic_cast<CloudBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_CLOUD);
//             } else if (dynamic_cast<EyesClosedBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_EYES_CLOSED);
//             } else if (dynamic_cast<EyesOpenedBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_EYES_OPENED);
//             } else if (dynamic_cast<GlassBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_GLASS);
//             } else if (dynamic_cast<QuestionBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_QUESTION);
//             } else if (dynamic_cast<WoodBlock*>(block)) {
//                 blockData.blockType = static_cast<int>(BlockType::BLOCK_WOOD);
//             }

//             blockData.x = block->GetPos().x;
//             blockData.y = block->GetPos().y;
//             blockData.hasBeenHit = block->isHit();
//             saveData.blocks.push_back(blockData);
//         }
//     }

//     // Thu thập tiles
//     for (const auto& tile : map.getInteractiveTiles()) {
//         if (tile) {
//             TileSave tileData;
//             tileData.x = tile->GetPos().x;
//             tileData.y = tile->GetPos().y;
//             tileData.tileType = tile->GetKey();
//             saveData.tiles.push_back(tileData);
//         }
//     }
// }

// void GameWorld::ApplyLoadedData(const GameSaveData& saveData) {
//     map.GetEnemies().clear();
//     map.GetInteractiveItems().clear();
//     map.getBlocks().clear();

//     for (const auto& enemyData : saveData.enemies) {
//         Enemy* enemy = nullptr;
//         switch (static_cast<EnemyType>(enemyData.enemyType)) {
//             case EnemyType::GOOMBA:
//                 enemy = new Goomba(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::GREENKOOPA:
//                 enemy = new GreenKoopa(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::REDKOOPA:
//                 enemy = new RedKoopa(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::REX:
//                 enemy = new Rex(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::YELLOWKOOPA:
//                 enemy = new YellowKoopa(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::BUZZYBEETLE:
//                 enemy = new BuzzyBeetle(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::FLYINGGOOMBA:
//                 enemy = new FlyingGoomba(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::BULLETBILL:
//                 enemy = new BulletBill(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::BANZAIBILL:
//                 enemy = new BanzaiBill(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::JUMPINGPIRANHA:
//                 enemy = new JumpingPiranhaPlant(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::PIRANHAPLANT:
//                 enemy = new PiranhaPlant(Vector2{enemyData.x, enemyData.y});
//                 break;
//             case EnemyType::BOSS: // Special ID for Boss
//                 // Handle boss initialization here if needed
//                 continue; // Skip boss for now
//         }
//         if (enemy) {
//             enemy->SetPos(Vector2{enemyData.x, enemyData.y});
//             enemy->SetVel(Vector2{enemyData.velX, enemyData.velY});
//             enemy->SetState(static_cast<ObjectState>(enemyData.state));
//             map.GetEnemies().push_back(enemy);
//         }
//     }
//     for (const auto& itemData : saveData.items) {
//         std::shared_ptr<Item> item = nullptr;
//         switch (static_cast<ItemType>(itemData.itemType)) {
//             case ItemType::COIN:
//                 item = std::make_shared<Coin>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::FIREFLOWER:
//                 item = std::make_shared<FireFlower>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::STAR:
//                 item = std::make_shared<Star>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::YOSHICOIN:
//                 item = std::make_shared<YoshiCoin>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::MUSHROOM:
//                 item = std::make_shared<Mushroom>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::THREEUPMOON:
//                 item = std::make_shared<ThreeUpMoon>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::ONEUPMUSHROOM:
//                 item = std::make_shared<OneUpMushroom>(Vector2{itemData.x, itemData.y});
//                 break;
//             case ItemType::COURSECLEARTOKEN:
//                 item = std::make_shared<CourseClearToken>(Vector2{itemData.x, itemData.y});
//                 break;
//         }
//         if (item) {
//             // item->SetCollected(itemData.isCollected); // Assuming you have a method to set collected state
//             map.GetInteractiveItems().push_back(item);
//         }
//     }
//     for (const auto& blockData : saveData.blocks) {
//         Block* block = nullptr;
//         switch (static_cast<BlockType>(blockData.blockType)) {
//             case BlockType::BLOCK_CLOUD:
//                 block = new CloudBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE);
//                 break;
//             case BlockType::BLOCK_EYES_CLOSED:
//                 block = new EyesClosedBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE);
//                 break;
//             case BlockType::BLOCK_EYES_OPENED:
//                 block = new EyesOpenedBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE);
//                 break;
//             case BlockType::BLOCK_GLASS:
//                 block = new GlassBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE);
//                 break;
//             case BlockType::BLOCK_QUESTION:
//                 block = new QuestionBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE, GIFT_COIN);
//                 break;
//             case BlockType::BLOCK_WOOD:
//                 block = new WoodBlock(Vector2{blockData.x, blockData.y}, Vector2{32, 32}, WHITE);
//                 break;
//         }
//         if (block) {
//             if (block->GetBlockType() == BlockType::BLOCK_EYES_OPENED && !blockData.hasBeenHit) {
//                 // block->setHit(false); // Assuming you have a method to set hit state
//             }
//             map.getBlocks().push_back(block);
//         }
//     }
// }

// // GameSaveData GameWorld::CreateSaveData() {
// //     GameSaveData saveData;
// //     CollectSaveData(saveData);
// //     return saveData;
// // }