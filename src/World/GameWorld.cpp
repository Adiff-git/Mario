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
    #include "Boss.h"
    GameWorld::GameWorld() : player1(nullptr), player2(nullptr), interactiveTiles(map.getInteractiveTiles())
{
    player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    map.LoadMap(3);
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
                     player2Character(p2Type)
{
    map.LoadMap(MapID);
    
    // Initialize Player 1
    if (p1Type == CharacterType::MARIO) {
        player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    } else {
        player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    }
    
    // Initialize Player 2 if multiplayer
    if (multiplayer) {
        if (p2Type == CharacterType::MARIO) {
            player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::WASD);
        } else {
            player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::WASD);
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
        map.GetInteractiveItems().push_back(std::make_shared<Coin>(Vector2{150, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<FireFlower>(Vector2{200, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<Star>(Vector2{350, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<ThreeUpMoon>(Vector2{400, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<YoshiCoin>(Vector2{450, 800}));
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
    if (player1) {
        player1->UpdateStateAndPhysic();
        player1->UpdateCollisionProbes();
        if (player1->GetState() != OBJECT_STATE_DEAD &&
            player1->GetState() != OBJECT_STATE_DYING &&
            player1->GetState() != OBJECT_STATE_VICTORY)
        {
            if (player1->GetPos().y > 900) // KIỂM TRA VỊ TRÍ PLAYER 1
            {
                player1->Die();
            }
            if (player1->GetPos().x > 1900) {
                player1->SetState(OBJECT_STATE_VICTORY);
                if (player2) {
                    player2->SetState(OBJECT_STATE_VICTORY); // Ensure both players reach victory state
                }
            }
            // Handle Player 1 collisions
            for (auto const &tile : interactiveTiles)
            {
                if (player1->checkCollisionType(*tile) != COLLISION_TYPE_NONE)

                    mediatorCollision.HandleCollision(player1, tile);
            }
            for (auto const &item : map.GetInteractiveItems()) {
                if (item && player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, item.get());
            }
            for (Enemy* enemy : map.GetEnemies()) {
                if (enemy && player1->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, enemy);
            }
        }
        
        if (player1->GetFireballs()) {
            for (auto& fireball : *player1->GetFireballs()) {
                if (fireball) {
                    for (auto const &tile : interactiveTiles) {
                        if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(fireball, tile);
                    }
                    for (Enemy* enemy : map.GetEnemies()) {
                        if (enemy && enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(enemy, fireball);
                    }
                }
            }
        }
    }
    
    // Update Player 2 (if multiplayer)
    if (isMultiplayer && player2) {
        player2->UpdateStateAndPhysic();
        player2->UpdateCollisionProbes();
        

        
        if (player2->GetState() != OBJECT_STATE_DEAD &&
            player2->GetState() != OBJECT_STATE_DYING &&
            player2->GetState() != OBJECT_STATE_VICTORY)
        {
            if (player2->GetPos().x > 1900) {
                player2->SetState(OBJECT_STATE_VICTORY);
                player1->SetState(OBJECT_STATE_VICTORY); // Ensure both players reach victory state
            }
            if (player2->GetPos().y > 900) // KIỂM TRA VỊ TRÍ PLAYER 2
        {
            player2->Die();
        }
            // Handle Player 2 collisions
            for (auto const &tile : interactiveTiles)
            {
                if (player2->checkCollisionType(*tile) != COLLISION_TYPE_NONE)

                    mediatorCollision.HandleCollision(player2, tile);
            }
            for (auto const &item : map.GetInteractiveItems()) {
                if (item && player2->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, item.get());
            }
            for (Enemy* enemy : map.GetEnemies()) {
                if (enemy && player2->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, enemy);
            }
        }
        
        if (player2->GetFireballs()) {
            for (auto& fireball : *player2->GetFireballs()) {
                if (fireball) {
                    for (auto const &tile : interactiveTiles) {
                        if (tile && fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(fireball, tile);
                    }
                    for (Enemy* enemy : map.GetEnemies()) {
                        if (enemy && enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                            mediatorCollision.HandleCollision(enemy, fireball);
                    }
                }
            }
        }
    }
    
    // Update Boss positions
    map.SetMarioPositionForBosses(player1->GetPosPtr(), player2 ? player2->GetPosPtr() : nullptr, isMultiplayer);
    
    // Enemy update
    for (Enemy* enemy : map.GetEnemies()) {
        if (enemy) {
            enemy->UpdateStateAndPhysic();
            for (auto const& tile : interactiveTiles) {
                if (tile && enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(enemy, tile);
            }
        }
    }
    
    // Item update
    for (auto &item : map.GetInteractiveItems()) {
        if (item) {
            item->Update();
            for (auto const &tile : interactiveTiles) {
                if (tile && item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(item.get(), tile);
            }
        }
    }
    
    // Handle blocks
    for (auto &block : map.getBlocks()) {
        if (block) {
            block->Update();
            if (player1) {
                CollisionType collision = block->checkCollisionType(*player1);
                if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED) {
                    if (!block->isHit()) player1->SetVel(Vector2{player1->GetVel().x, 0});
                    block->doHit(*player1, this->GetMap());
                }
                if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION) {
                    block->doHit(*player1, this->GetMap());
                    player1->SetVel({player1->GetVel().x, 0});
                }
                if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS) {
                    block->doHit(*player1, this->GetMap());
                    player1->SetVel({player1->GetVel().x, 0});
                }
                mediatorCollision.HandleCollision(player1, block);
            }
            if (isMultiplayer && player2) {
                CollisionType collision2 = block->checkCollisionType(*player2);
                if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED) {
                    if (!block->isHit()) player2->SetVel(Vector2{player2->GetVel().x, 0});
                    block->doHit(*player2, this->GetMap());
                }
                if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION) {
                    block->doHit(*player2, this->GetMap());
                    player2->SetVel({player2->GetVel().x, 0});
                }
                if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS) {
                    block->doHit(*player2, this->GetMap());
                    player2->SetVel({player2->GetVel().x, 0});
                }
                mediatorCollision.HandleCollision(player2, block);
            }
        }
    }
    
    // Cleanup
    auto& blocks = map.getBlocks();
    blocks.erase(
        std::remove_if(
            blocks.begin(),
            blocks.end(),
            [](Block* block) {
                return (!block || block->GetState() == OBJECT_STATE_TO_BE_REMOVED);
            }
        ),
        blocks.end()
    );
    
    auto &interactiveItems = map.GetInteractiveItems();
    interactiveItems.erase(
        std::remove_if(
            interactiveItems.begin(),
            interactiveItems.end(),
            [](const std::shared_ptr<Item>& item) {
                return (!item || item->GetState() == OBJECT_STATE_TO_BE_REMOVED || item->GetState() == OBJECT_STATE_DEAD);
            }
        ),
        interactiveItems.end()
    );
    
    map.GetEnemies().erase(
        std::remove_if(
            map.GetEnemies().begin(),
            map.GetEnemies().end(),
            [](Enemy* enemy) {
                return (!enemy || enemy->GetState() == OBJECT_STATE_DEAD || enemy->GetState() == OBJECT_STATE_TO_BE_REMOVED);
            }
        ),
        map.GetEnemies().end()
    );
    
    // Game state logic
    bool player1Dead = (player1 && player1->GetState() == OBJECT_STATE_DEAD);
    bool player2Dead = (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_DEAD);
    
    if (isMultiplayer) {
        if (player1Dead && player2Dead) {
            if ((player1 && player1->GetLives() > 0) || (player2 && player2->GetLives() > 0)) {
                gameState = GameState::GAME_RESET;
            } else {
                gameState = GameState::GAME_OVER;
            }
        }
    } else {
        if (player1Dead) {
            if (player1 && player1->GetLives() > 1) {
                gameState = GameState::GAME_RESET;
            } else {
                gameState = GameState::GAME_OVER;
            }
        }
    }
    
    if ((player1 && player1->GetState() == OBJECT_STATE_VICTORY) || 
        (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_VICTORY)) {
        gameState = GameState::GAME_COMPLETED;
    }
    cout << player1->GetPos().x ;
}

void GameWorld::DrawWorld()
{
    if (isMultiplayer && player1 && player2) {
        Vector2 p1 = player1->GetPos();
        Vector2 p2 = player2->GetPos();
        float avgX = (p1.x + p2.x) / 2.0f;
        float avgY = (p1.y + p2.y) / 2.0f;
        float dx = fabsf(p1.x - p2.x);
        float dy = fabsf(p1.y - p2.y);
        float maxDist = 700.0f;
        float minZoom = 0.7f;
        float maxZoom = 1.3f;
        float dist = sqrtf(dx*dx + dy*dy);
        float zoom = maxZoom - (dist / maxDist) * (maxZoom - minZoom);
        if (zoom < minZoom) zoom = minZoom;
        if (zoom > maxZoom) zoom = maxZoom;
        camera.target = { avgX, avgY };
        camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        camera.zoom = zoom;
        
        float camLeft = camera.target.x - (GetScreenWidth() / 2) / camera.zoom;
        float camRight = camera.target.x + (GetScreenWidth() / 2) / camera.zoom;
        float camTop = camera.target.y - (GetScreenHeight() / 2) / camera.zoom;
        float camBottom = camera.target.y + (GetScreenHeight() / 2) / camera.zoom;
        
        if (camLeft < 0) camera.target.x = (GetScreenWidth() / 2) / camera.zoom;
        if (camRight > map.GetWidth()) camera.target.x = map.GetWidth() - (GetScreenWidth() / 2) / camera.zoom;
        if (camTop < 0) camera.target.y = (GetScreenHeight() / 2) / camera.zoom;
        if (camBottom > map.getHeight()) camera.target.y = map.getHeight() - (GetScreenHeight() / 2) / camera.zoom;
    } else if (player1) {
        camera.target = player1->GetPos();
        camera.offset = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
        camera.zoom = 1.3f;
        
        float camLeft = camera.target.x - (GetScreenWidth() / 2) / camera.zoom;
        float camRight = camera.target.x + (GetScreenWidth() / 2) / camera.zoom;
        float camTop = camera.target.y - (GetScreenHeight() / 2) / camera.zoom;
        float camBottom = camera.target.y + (GetScreenHeight() / 2) / camera.zoom;
        
        if (camLeft < 0) camera.target.x = (GetScreenWidth() / 2) / camera.zoom;
        if (camRight > map.GetWidth()) camera.target.x = map.GetWidth() - (GetScreenWidth() / 2) / camera.zoom;
        if (camTop < 0) camera.target.y = (GetScreenHeight() / 2) / camera.zoom;
        if (camBottom > map.getHeight()) camera.target.y = map.getHeight() - (GetScreenHeight() / 2) / camera.zoom;
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