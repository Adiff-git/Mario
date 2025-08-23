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
    printf("[GameWorld] Default constructor called\n");
    player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    map.LoadMap(5);
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");

    InitializeSpatialIndex();
    RebuildSpatialIndex();
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
    printf("[GameWorld] Constructor called: MapID=%d, multiplayer=%d\n", MapID, multiplayer);

    // Clear and delete old enemies (especially Boss)
    auto& enemies = map.GetEnemies();
    for (auto* enemy : enemies) {
        if (enemy) delete enemy;
    }
    enemies.clear();

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
            switch(p2Type) {
                case CharacterType::MARIO:
                    player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                    break;
                case CharacterType::LUIGI:
                    player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                    break;
                case CharacterType::TOAD:
                    player2 = new Toad(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                    break;
                case CharacterType::PEACH:
                    player2 = new Peach(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                    break;
            }
        }
    
    // Initialize Boss for MapID == 1
    
    
    // Set background based on MapIDz
    switch (MapID)
    {
        case 0: background = ResrcManager::GetInstance().getTexture("BACKGROUND_0"); break;
        case 1: background = ResrcManager::GetInstance().getTexture("BACKGROUND_1"); break;
        case 2: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 3: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
        case 4: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 5: background = ResrcManager::GetInstance().getTexture("BACKGROUND_5"); break;
        case 6: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 7: background = ResrcManager::GetInstance().getTexture("BACKGROUND_7"); break;
        case 8: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 9: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
    }
    if (selectedMapId == 4) {
        Boss* boss = new Boss(Vector2{2300, 600}, player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
        map.GetEnemies().push_back(boss);
        map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
    }
    
    // Add items for MapID == 1
    // if (MapID == 1) {
        // map.GetInteractiveItems().push_back(std::make_shared<Coin>(Vector2{400, 800}));



    // }
    
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
    printf("[GameWorld] Constructor called (speedMultiplier): MapID=%d, multiplayer=%d, speedMultiplier=%.2f\n", MapID, multiplayer, speedMultiplier);

    // Clear and delete old enemies (especially Boss)
    auto& enemies = map.GetEnemies();
    for (auto* enemy : enemies) {
        if (enemy) delete enemy;
    }
    enemies.clear();

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
        switch(p2Type) {
            case CharacterType::MARIO:
                player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                break;
            case CharacterType::LUIGI:
                player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                break;
            case CharacterType::TOAD:
                player2 = new Toad(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                break;
            case CharacterType::PEACH:
                player2 = new Peach(Vector2{150, 100}, 3, SMALL, ControlType::ARROWS);
                break;
        }
    }
    
    switch (MapID)
    {
        case 0: background = ResrcManager::GetInstance().getTexture("BACKGROUND_0"); break;
        case 1: background = ResrcManager::GetInstance().getTexture("BACKGROUND_1"); break;
        case 2: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 3: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
        case 4: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 5: background = ResrcManager::GetInstance().getTexture("BACKGROUND_5"); break;
        case 6: background = ResrcManager::GetInstance().getTexture("BACKGROUND_6"); break;
        case 7: background = ResrcManager::GetInstance().getTexture("BACKGROUND_7"); break;
        case 8: background = ResrcManager::GetInstance().getTexture("BACKGROUND_8"); break;
        case 9: background = ResrcManager::GetInstance().getTexture("BACKGROUND_9"); break;
    }

    if (MapID == 4) {
        Boss* boss = new Boss(Vector2{2300, 600}, player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
        map.GetEnemies().push_back(boss);
        printf("[GameWorld] Boss pushed to enemy list at (%.1f, %.1f)\n", boss->GetPos().x, boss->GetPos().y);
        map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, multiplayer);
    }
    
    
    
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitializeSpatialIndex();
    RebuildSpatialIndex();
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
    printf("[GameWorld] Constructor called (level): level=%d\n", level);
    map.LoadMap(level);
    
    player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    player2 = nullptr;
    
    map.SetMarioPositionForBosses(player1->GetPosPtr(), nullptr, false);
    
    camera.target = Vector2{player1->GetPos().x, player1->GetPos().y};
    camera.offset = Vector2{400, 300};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    InitializeSpatialIndex();
    RebuildSpatialIndex();
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
    RebuildSpatialIndex();

    std::vector<Block*> activeBlocks;
    std::vector<Enemy*> activeEnemies;
    std::vector<std::shared_ptr<Item>> activeItems;
    std::vector<Tile*> activeTiles;
    CollectActiveEntities(activeBlocks, activeEnemies, activeItems, activeTiles);

    // -------- Thay interactiveTiles / map collections bằng active* --------

    if (player1)
    {
        player1->UpdateStateAndPhysic();
        player1->UpdateCollisionProbes();
        ScoreEffect::SetCurrentPlayer(player1);
        if (player1->GetState() != OBJECT_STATE_DEAD &&
            player1->GetState() != OBJECT_STATE_DYING &&
            player1->GetState() != OBJECT_STATE_VICTORY)
        {
            // if (player1->GetPos().y > 900) player1->Die();

            // Tiles
            for (auto const &tile : activeTiles)
            {
                if (player1->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, tile);
            }
            // Items
            for (auto const &item : activeItems)
            {
                if (item && player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, item.get());
                    // Nếu là CourseClearToken và player nhặt được thì chuyển trạng thái game
                    if (item && dynamic_cast<CourseClearToken*>(item.get()) && player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    {
                        gameState = GameState::GAME_COMPLETED;
                        item->SetState(OBJECT_STATE_DEAD); // Ẩn token sau khi nhặt
                    }
            }
            // Enemies
            for (Enemy *enemy : activeEnemies)
            {
                if (enemy && enemy->GetState() != OBJECT_STATE_DEAD && enemy->GetState() != OBJECT_STATE_DYING &&
                    enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                    player1->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                {
                    if (Boss *boss = dynamic_cast<Boss *>(enemy))
                        if (boss->IsDead()) continue;
                    mediatorCollision.HandleCollision(player1, enemy);
                }
                if (Boss *boss = dynamic_cast<Boss *>(enemy))
                {
                    for (BossFireball *fireball : boss->getProjectiles())
                    {
                        if (fireball && fireball->GetState() != OBJECT_STATE_DEAD &&
                            player1->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            mediatorCollision.HandleCollision(player1, fireball);
                            fireball->SetState(OBJECT_STATE_DEAD);
                        }
                    }
                }
            }
            // Blocks
            for (auto &block : activeBlocks)
            {
                if (block)
                {
                    CollisionType collision = block->checkCollisionType(*player1);
                    if (collision == COLLISION_TYPE_SOUTH)
                    {
                        if (block->GetBlockType() == BlockType::BLOCK_EYES_OPENED)
                        {
                            if (!block->isHit())
                                player1->SetVel(Vector2{player1->GetVel().x, 0});
                            block->doHit(*player1, this->GetMap());
                        }
                        else if (block->GetBlockType() == BlockType::BLOCK_QUESTION ||
                                 block->GetBlockType() == BlockType::BLOCK_GLASS)
                        {
                            block->doHit(*player1, this->GetMap());
                            player1->SetVel({player1->GetVel().x, 0});
                        }
                    }
                    mediatorCollision.HandleCollision(player1, block);
                }
            }
        }
        float deathY = (float)map.getHeight() + 64.0f; // margin nhỏ
            if (player1->GetPos().y > deathY) player1->Die();
        // Player1 fireballs
        if (player1->GetFireballs())
        {
            for (auto &fireball : *player1->GetFireballs())
            {
                if (!fireball) continue;
                for (auto const &tile : activeTiles)
                {
                    if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                        mediatorCollision.HandleCollision(fireball, tile);
                }
                for (auto const &block : activeBlocks)
                {
                    if (block && fireball->checkCollisionType(*block) != COLLISION_TYPE_NONE)
                    {
                        mediatorCollision.HandleCollision(fireball, block);
                    }
                }
                for (Enemy *enemy : activeEnemies)
                {
                    if (enemy && enemy->GetState() != OBJECT_STATE_DEAD && enemy->GetState() != OBJECT_STATE_DYING &&
                        enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                        enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                    {
                        if (Boss *boss = dynamic_cast<Boss *>(enemy))
                            if (boss->IsDead()) continue;
                        mediatorCollision.HandleCollision(enemy, fireball);
                    }
                }
            }
        }
    }

    // Player 2 (multiplayer)
    if (isMultiplayer && player2)
    {
        player2->UpdateStateAndPhysic();
        player2->UpdateCollisionProbes();
        if (player2->GetState() != OBJECT_STATE_DEAD &&
            player2->GetState() != OBJECT_STATE_DYING &&
            player2->GetState() != OBJECT_STATE_VICTORY)
        {
            // if (player2->GetPos().y > 900) player2->Die();

            for (auto const &tile : activeTiles)
            {
                if (player2->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, tile);
            }
            for (auto const &item : activeItems)
            {
                if (item && player2->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, item.get());
                    // Nếu là CourseClearToken và player2 nhặt được thì chuyển trạng thái game
                    if (item && dynamic_cast<CourseClearToken*>(item.get()) && player2->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    {
                        gameState = GameState::GAME_COMPLETED;
                        item->SetState(OBJECT_STATE_DEAD);
                    }
            }
            for (Enemy *enemy : activeEnemies)
            {
                if (enemy && enemy->GetState() != OBJECT_STATE_DEAD && enemy->GetState() != OBJECT_STATE_DYING &&
                    enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                    player2->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                {
                    if (Boss *boss = dynamic_cast<Boss *>(enemy))
                        if (boss->IsDead()) continue;
                    mediatorCollision.HandleCollision(player2, enemy);
                }
                if (Boss *boss = dynamic_cast<Boss *>(enemy))
                {
                    for (BossFireball *fireball : boss->getProjectiles())
                    {
                        if (fireball && fireball->GetState() != OBJECT_STATE_DEAD &&
                            player2->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                        {
                            mediatorCollision.HandleCollision(player2, fireball);
                            fireball->SetState(OBJECT_STATE_DEAD);
                        }
                    }
                }
            }
            for (auto &block : activeBlocks)
            {
                if (block)
                {
                    CollisionType collision2 = block->checkCollisionType(*player2);
                    if (collision2 == COLLISION_TYPE_SOUTH)
                    {
                        if (block->GetBlockType() == BlockType::BLOCK_EYES_OPENED)
                        {
                            if (!block->isHit())
                                player2->SetVel(Vector2{player2->GetVel().x, 0});
                            block->doHit(*player2, this->GetMap());
                        }
                        else if (block->GetBlockType() == BlockType::BLOCK_QUESTION ||
                                 block->GetBlockType() == BlockType::BLOCK_GLASS)
                        {
                            block->doHit(*player2, this->GetMap());
                            player2->SetVel({player2->GetVel().x, 0});
                        }
                    }
                    mediatorCollision.HandleCollision(player2, block);
                }
            }
        }
        float deathY2 = (float)map.getHeight() + 64.0f;
            if (player2->GetPos().y > deathY2) player2->Die();
        if (player2->GetFireballs())
        {
            for (auto &fireball : *player2->GetFireballs())
            {
                if (!fireball) continue;
                for (auto const &tile : activeTiles)
                {
                    if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                        mediatorCollision.HandleCollision(fireball, tile);
                }
                for (auto const &block : activeBlocks)
                {
                    if (block && fireball->checkCollisionType(*block) != COLLISION_TYPE_NONE)
                    {
                        mediatorCollision.HandleCollision(fireball, block);
                    }
                }
                for (Enemy *enemy : activeEnemies)
                {
                    if (enemy && enemy->GetState() != OBJECT_STATE_DEAD && enemy->GetState() != OBJECT_STATE_DYING &&
                        enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                        enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                    {
                        if (Boss *boss = dynamic_cast<Boss *>(enemy))
                            if (boss->IsDead()) continue;
                        mediatorCollision.HandleCollision(enemy, fireball);
                    }
                }
            }
        }
    }

    // Boss follow players
    map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, isMultiplayer);

    // Enemy update (only active)
    for (Enemy *enemy : activeEnemies)
    {
        if (!enemy) continue;
        if (Boss *boss = dynamic_cast<Boss *>(enemy))
            if (boss->IsDead()) continue;

        enemy->UpdateDyingState();
        enemy->UpdateDeathEffect();

        if (enemy->GetState() != OBJECT_STATE_DYING &&
            enemy->GetState() != OBJECT_STATE_DEAD &&
            enemy->GetState() != OBJECT_STATE_TO_BE_REMOVED)
        {
            enemy->UpdateStateAndPhysic();
            for (auto const &tile : activeTiles)
            {
                if (tile && enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(enemy, tile);
            }
            for (auto const &block : activeBlocks)
            {
                if (block && enemy->checkCollisionType(*block) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(enemy, block);
            }
        }
    }
    // Enemy vs Enemy collisions
    auto& enemies = map.GetEnemies();
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        for (size_t j = i + 1; j < enemies.size(); ++j)
        {
            Enemy* enemyA = enemies[i];
            Enemy* enemyB = enemies[j];
            if (enemyA && enemyB &&
                enemyA->GetState() != OBJECT_STATE_DEAD &&
                enemyA->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                enemyB->GetState() != OBJECT_STATE_DEAD &&
                enemyB->GetState() != OBJECT_STATE_TO_BE_REMOVED &&
                enemyA->checkCollisionType(*enemyB) != COLLISION_TYPE_NONE)
            {
                mediatorCollision.HandleCollision(enemyA, enemyB);
            }
        }
    }

    // Item update (only active)
    for (auto &item : activeItems)
    {
        if (!item) continue;
        item->Update();
        for (auto const &tile : activeTiles)
        {
            if (tile && item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(item.get(), tile);
        }
        for (auto const &block : activeBlocks)
        {
            if (block && item->checkCollisionType(*block) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(item.get(), block);
        }
    }

    // Blocks update (only active)
    for (auto &block : activeBlocks)
    {
        if (block) block->Update();
        
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
            [this](Enemy *enemy)
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
                        // Spawn CourseClearToken at boss position
                        auto &items = map.GetInteractiveItems();
                        bool tokenExists = false;
                        for (auto& item : items) {
                            if (item && dynamic_cast<CourseClearToken*>(item.get())) {
                                tokenExists = true;
                                break;
                            }
                        }
                        if (!tokenExists) {
                                items.push_back(std::make_shared<CourseClearToken>(Vector2{2000, 800}));
                        }
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
            if ((player1 && player1->GetLives() > 1) || (player2 && player2->GetLives() > 1))
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

// void GameWorld::DrawWorld()
// {
//     // Camera state: hysteresis zoom
//     static bool  camInit     = false;
//     static float camZoom     = 1.1f;   // current zoom (MP)
//     static float targetZoom  = 1.1f;   // desired zoom (MP)
//     // Zoom limits
//     const float minZoom      = 1.0f;   // zoom out tối đa
//     const float maxZoom      = 1.25f;  // zoom in tối đa
//     // Hysteresis theo khoảng cách giữa 2 người chơi
//     const float nearDist     = 420.0f; // < nearDist => zoom in
//     const float farDist      = 760.0f; // > farDist  => zoom out
//     const float zoomSmooth   = 0.12f;  // 0..1

//     auto clampf = [](float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); };

//     if (!camInit) {
//         camZoom    = clampf(camera.zoom, minZoom, maxZoom);
//         targetZoom = camZoom;
//         // Lọc texture nền để giảm rung hình khi zoom
//         SetTextureFilter(background, TEXTURE_FILTER_BILINEAR);
//         camInit    = true;
//     }

//     camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

//     if (isMultiplayer && player1 && player2)
//     {
//         Vector2 p1 = player1->GetPos();
//         Vector2 p2 = player2->GetPos();

//         // Khoảng cách dùng cho hysteresis
//         float dx = fabsf(p1.x - p2.x);
//         float dy = fabsf(p1.y - p2.y);
//         float dist = sqrtf(dx * dx + dy * dy);

//         if (dist > farDist)       targetZoom = minZoom;
//         else if (dist < nearDist) targetZoom = maxZoom;

//         camZoom = camZoom + (targetZoom - camZoom) * zoomSmooth;
//         camera.zoom = clampf(camZoom, minZoom, maxZoom);

//         // Tâm camera là tâm 2 người chơi (không khóa trục dọc)
//         Vector2 center{ (p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f };
//         camera.target = center;

//         // Clamp trong biên map theo world-units (có xét zoom)
//         float halfW = (GetScreenWidth()  * 0.5f) / camera.zoom;
//         float halfH = (GetScreenHeight() * 0.5f) / camera.zoom;
//         float mapW  = (float)map.GetWidth();
//         float mapH  = (float)map.getHeight();

//         if (camera.target.x - halfW < 0)    camera.target.x = halfW;
//         if (camera.target.x + halfW > mapW) camera.target.x = mapW - halfW;
//         if (camera.target.y - halfH < 0)    camera.target.y = halfH;
//         if (camera.target.y + halfH > mapH) camera.target.y = mapH - halfH;
//     }
//     else if (player1)
//     {
//         // Single player: zoom cố định, theo dõi cả trục dọc
//         camera.zoom = 1.3f;

//         Vector2 center = player1->GetPos();
//         camera.target  = center;

//         float halfW = (GetScreenWidth()  * 0.5f) / camera.zoom;
//         float halfH = (GetScreenHeight() * 0.5f) / camera.zoom;
//         float mapW  = (float)map.GetWidth();
//         float mapH  = (float)map.getHeight();

//         if (camera.target.x - halfW < 0)    camera.target.x = halfW;
//         if (camera.target.x + halfW > mapW) camera.target.x = mapW - halfW;
//         if (camera.target.y - halfH < 0)    camera.target.y = halfH;
//         if (camera.target.y + halfH > mapH) camera.target.y = mapH - halfH;
//     }

//     // Parallax background wrap theo world-units (có xét zoom) để tránh lỗi khi zoom dọc
//     float halfW = (GetScreenWidth() * 0.5f) / camera.zoom;
//     float tileW = background.width * 1.3f;

//     // Dịch BGpos theo hướng camera di chuyển, dùng while để phòng trường hợp nhảy xa
//     while (camera.target.x - halfW >= BGpos + tileW) BGpos += tileW;
//     while (camera.target.x + halfW <= BGpos)         BGpos -= tileW;

//     BeginMode2D(camera);
//     DrawTextureEx(background, Vector2{BGpos - tileW, -200}, 0.0f, 1.3f, WHITE);
//     DrawTextureEx(background, Vector2{BGpos,         -200}, 0.0f, 1.3f, WHITE);
//     DrawTextureEx(background, Vector2{BGpos + tileW, -200}, 0.0f, 1.3f, WHITE);
//     map.Draw();

//     if (player1) player1->Draw();
//     if (isMultiplayer && player2) player2->Draw();
//     EndMode2D();
// }


void GameWorld::DrawWorld()
{
    // Camera state: hysteresis zoom
    static bool  camInit     = false;
    static float camZoom     = 1.1f;   // current zoom (MP)
    static float targetZoom  = 1.1f;   // desired zoom (MP)
    // Zoom limits
    const float minZoom      = 1.0f;   // zoom out tối đa
    const float maxZoom      = 1.25f;  // zoom in tối đa
    // Hysteresis theo khoảng cách giữa 2 người chơi
    const float nearDist     = 420.0f; // < nearDist => zoom in
    const float farDist      = 760.0f; // > farDist  => zoom out
    const float zoomSmooth   = 0.12f;  // 0..1

    auto clampf = [](float v, float lo, float hi){ return v < lo ? lo : (v > hi ? hi : v); };
    auto isAlive = [](Character* p){
        if (!p) return false;
        auto s = p->GetState();
        return s != OBJECT_STATE_DEAD && s != OBJECT_STATE_DYING;
    };

    if (!camInit) {
        camZoom    = clampf(camera.zoom, minZoom, maxZoom);
        targetZoom = camZoom;
        // Lọc texture nền để giảm rung hình khi zoom
        SetTextureFilter(background, TEXTURE_FILTER_BILINEAR);
        camInit    = true;
    }

    camera.offset = {(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};

    if (isMultiplayer && player1 && player2)
    {
        bool p1Alive = isAlive(player1);
        bool p2Alive = isAlive(player2);

        if (p1Alive && p2Alive)
        {
            Vector2 p1 = player1->GetPos();
            Vector2 p2 = player2->GetPos();

            // Khoảng cách dùng cho hysteresis
            float dx = fabsf(p1.x - p2.x);
            float dy = fabsf(p1.y - p2.y);
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist > farDist)       targetZoom = minZoom;
            else if (dist < nearDist) targetZoom = maxZoom;

            camZoom = camZoom + (targetZoom - camZoom) * zoomSmooth;
            camera.zoom = clampf(camZoom, minZoom, maxZoom);

            // Tâm camera là tâm 2 người chơi
            Vector2 center{ (p1.x + p2.x) * 0.5f, (p1.y + p2.y) * 0.5f };
            camera.target = center;
        }
        else
        {
            // Chỉ còn 1 người sống: focus vào người còn lại và zoom-in nhẹ để ổn định khung hình
            Character* focus = p1Alive ? player1 : (p2Alive ? player2 : player1);
            targetZoom = maxZoom; // zoom gần để tránh cộng hưởng zoom dọc khi chỉ còn 1 nhân vật
            camZoom = camZoom + (targetZoom - camZoom) * zoomSmooth;
            camera.zoom = clampf(camZoom, minZoom, maxZoom);

            camera.target = focus->GetPos();
        }

        // Clamp trong biên map theo world-units (có xét zoom)
        float halfW = (GetScreenWidth()  * 0.5f) / camera.zoom;
        float halfH = (GetScreenHeight() * 0.5f) / camera.zoom;
        float mapW  = (float)map.GetWidth();
        float mapH  = (float)map.getHeight();

        if (camera.target.x - halfW < 0)    camera.target.x = halfW;
        if (camera.target.x + halfW > mapW) camera.target.x = mapW - halfW;
        if (camera.target.y - halfH < 0)    camera.target.y = halfH;
        if (camera.target.y + halfH > mapH) camera.target.y = mapH - halfH;
    }
    else if (player1)
    {
        // Single player: zoom cố định, theo dõi cả trục dọc
        camera.zoom = 1.3f;

        Vector2 center = player1->GetPos();
        camera.target  = center;

        float halfW = (GetScreenWidth()  * 0.5f) / camera.zoom;
        float halfH = (GetScreenHeight() * 0.5f) / camera.zoom;
        float mapW  = (float)map.GetWidth();
        float mapH  = (float)map.getHeight();

        if (camera.target.x - halfW < 0)    camera.target.x = halfW;
        if (camera.target.x + halfW > mapW) camera.target.x = mapW - halfW;
        if (camera.target.y - halfH < 0)    camera.target.y = halfH;
        if (camera.target.y + halfH > mapH) camera.target.y = mapH - halfH;
    }

    // Parallax background wrap theo world-units (có xét zoom)
    float halfW = (GetScreenWidth() * 0.5f) / camera.zoom;
    float tileW = background.width * 1.3f;

    while (camera.target.x - halfW >= BGpos + tileW) BGpos += tileW;
    while (camera.target.x + halfW <= BGpos)         BGpos -= tileW;

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{BGpos - tileW, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos,         -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos + tileW, -200}, 0.0f, 1.3f, WHITE);
    map.Draw();

        // Vẽ tất cả item (bao gồm CourseClearToken)
        for (auto& item : map.GetInteractiveItems()) {
            if (item && item->GetState() != OBJECT_STATE_DEAD) {
                item->Draw();
            }
        }

        if (player1) player1->Draw();
        if (isMultiplayer && player2) player2->Draw();
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

            
// ===========================================================================================================

void GameWorld::InitializeSpatialIndex() {
    int mapW = map.GetWidth();
    int mapH = map.getHeight();
    gridCols = (mapW + CELL_SIZE - 1) / CELL_SIZE;
    gridRows = (mapH + CELL_SIZE - 1) / CELL_SIZE;
    spatialGrid.assign(gridRows, std::vector<SpatialCell>(gridCols));
}

void GameWorld::ClearSpatialGrid() {
    for (int r = 0; r < gridRows; ++r) {
        for (int c = 0; c < gridCols; ++c) {
            spatialGrid[r][c].blocks.clear();
            spatialGrid[r][c].enemies.clear();
            spatialGrid[r][c].items.clear();
            spatialGrid[r][c].tiles.clear();
        }
    }
}

template<typename TObj, typename FnPos>
void GameWorld::InsertObjectToGrid(TObj* obj, FnPos getPosFn) {
    if (!obj) return;
    Vector2 p = getPosFn(obj);
    if (p.x < 0 || p.y < 0) return;
    int cx = (int)(p.x) / CELL_SIZE;
    int cy = (int)(p.y) / CELL_SIZE;
    if (cx < 0 || cy < 0 || cx >= gridCols || cy >= gridRows) return;
    // phân loại theo kiểu
    if constexpr (std::is_same_v<TObj, Block>)
        spatialGrid[cy][cx].blocks.push_back(static_cast<Block*>(obj));
    else if constexpr (std::is_same_v<TObj, Enemy>)
        spatialGrid[cy][cx].enemies.push_back(static_cast<Enemy*>(obj));
    else if constexpr (std::is_same_v<TObj, Tile>)
        spatialGrid[cy][cx].tiles.push_back(static_cast<Tile*>(obj));
    // Item dùng shared_ptr -> xử lý riêng trong Rebuild
}

void GameWorld::RebuildSpatialIndex() {
    if (gridCols == 0 || gridRows == 0)
        InitializeSpatialIndex();

    ClearSpatialGrid();

    // Tiles (interactiveTiles là static)
    for (auto* tile : interactiveTiles) {
        InsertObjectToGrid<Tile>(tile, [](Tile* t){ return t->GetPos(); });
    }

    // Blocks
    for (auto* blk : map.getBlocks()) {
        InsertObjectToGrid<Block>(blk, [](Block* b){ return b->GetPos(); });
    }

    // Enemies
    for (auto* e : map.GetEnemies()) {
        InsertObjectToGrid<Enemy>(e, [](Enemy* en){ return en->GetPos(); });
    }

    // Items (shared_ptr)
    for (auto &it : map.GetInteractiveItems()) {
        if (!it) continue;
        Vector2 p = it->GetPos();
        int cx = (int)p.x / CELL_SIZE;
        int cy = (int)p.y / CELL_SIZE;
        if (cx >=0 && cy >=0 && cx < gridCols && cy < gridRows) {
            spatialGrid[cy][cx].items.push_back(it);
        }
    }
}

void GameWorld::CollectActiveEntities(std::vector<Block*>& outBlocks,
                                      std::vector<Enemy*>& outEnemies,
                                      std::vector<std::shared_ptr<Item>>& outItems,
                                      std::vector<Tile*>& outTiles) {
    outBlocks.clear();
    outEnemies.clear();
    outItems.clear();
    outTiles.clear();

    float vw = (float)GetScreenWidth();
    float vh = (float)GetScreenHeight();
    float left = camera.target.x - vw * 0.5f - ACTIVE_MARGIN;
    float top  = camera.target.y - vh * 0.5f - ACTIVE_MARGIN;
    float right = camera.target.x + vw * 0.5f + ACTIVE_MARGIN;
    float bottom = camera.target.y + vh * 0.5f + ACTIVE_MARGIN;

    int c0 = std::max(0, (int)floor(left / CELL_SIZE));
    int r0 = std::max(0, (int)floor(top / CELL_SIZE));
    int c1 = std::min(gridCols - 1, (int)floor(right / CELL_SIZE));
    int r1 = std::min(gridRows - 1, (int)floor(bottom / CELL_SIZE));

    // Tránh duplicate (object có thể nằm trùng cell nếu to) – ở đây object point duy nhất -> bỏ qua set để nhẹ.
    for (int r = r0; r <= r1; ++r) {
        for (int c = c0; c <= c1; ++c) {
            auto &cell = spatialGrid[r][c];
            outBlocks.insert(outBlocks.end(), cell.blocks.begin(), cell.blocks.end());
            outEnemies.insert(outEnemies.end(), cell.enemies.begin(), cell.enemies.end());
            outItems.insert(outItems.end(), cell.items.begin(), cell.items.end());
            outTiles.insert(outTiles.end(), cell.tiles.begin(), cell.tiles.end());
        }
    }
}

