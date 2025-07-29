#include "GameWorld.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "Star.h"
#include "ThreeUpMoon.h"
#include "Luigi.h"
#include "YoshiCoin.h"
#include "EnemyManager.h"
#include "MediatorCollision.h"
#include "Mario.h"
GameWorld::GameWorld() : player1(nullptr),player2(nullptr),
interactiveTiles(map.getInteractiveTiles())
{
    // Trong GameWorld constructor, thêm:

    player1 =  new Luigi(Vector2{100, 100}, 3, SMALL); // Đặt vị trí cụ thể
    map.LoadMap(0);
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");
}

GameWorld::GameWorld(int MapID, GameScreen *gameScreen, bool multiplayer1,
                        CharacterType p1Type, CharacterType p2Type) : 
                                                        player1(nullptr), player2(nullptr),
                                                          interactiveTiles(map.getInteractiveTiles()),
                                                          gameScreen(gameScreen),
                                                          gameState(GameState::GAME_PLAYING),
                                                            isMultiplayer(multiplayer1),
                                                            player1Character(p1Type),
                                                            player2Character(p2Type)
{
    map.LoadMap(MapID);
    
    // Initialize Player1 1
    if (p1Type == CharacterType::MARIO) {
        player1 = new Mario(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    } else {
        player1 = new Luigi(Vector2{100, 100}, 3, SMALL, ControlType::ARROWS);
    }
    
    // Initialize Player1 2 if multiplayer1
    if (multiplayer1) {
        if (p2Type == CharacterType::MARIO) {
            player2 = new Mario(Vector2{150, 100}, 3, SMALL, ControlType::WASD);
        } else {
            player2 = new Luigi(Vector2{150, 100}, 3, SMALL, ControlType::WASD);
        }
    }


    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player1->GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    switch (MapID)
    {
    case 0:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_0");
        break;
    case 1:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_1");
        break;
    case 2:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_2");
        break;
    case 3:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_3");
        break;
    case 4:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_4");
        break;
    case 5:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_5");
        break;
    case 6:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_6");
        break;
    case 7:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_7");
        break;
    case 8:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_8");
        break;
    case 9:
        background = ResrcManager::GetInstance().getTexture("BACKGROUND_9");
        break;
    }
    if (MapID == 1)
    {
        map.GetInteractiveItems().push_back(std::make_shared<Coin>(Vector2{150, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<CourseClearToken>(Vector2{200, 800}));
        map.GetInteractiveItems().push_back(std::make_shared<FireFlower>(Vector2{200, 800}));
        // interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{250, 500}));
        // interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{300, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<Star>(Vector2{350, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<ThreeUpMoon>(Vector2{400, 500}));
        map.GetInteractiveItems().push_back(std::make_shared<YoshiCoin>(Vector2{450, 800}));

        map.GetEnemies().push_back(new Goomba(Vector2{450, 500}));
        map.GetEnemies().push_back(new GreenKoopa(Vector2{500, 500}));
        map.GetEnemies().push_back(new BuzzyBeetle(Vector2{600, 500}));
        map.GetEnemies().push_back(new Rex(Vector2{650, 500}));
        map.GetEnemies().push_back(new FlyingGoomba(Vector2{750, 500}));
    }
}

GameWorld::~GameWorld()
{
    // Destructor logic if needed
}



// void GameWorld::UpdateWorld()
// {
//     player1->UpdateStateAndPhysic();
//     player1->UpdateCollisionProbes();

//     if (player1->GetState() != OBJECT_STATE_DEAD &&
//         player1->GetState() != OBJECT_STATE_DYING &&
//         player1->GetState() != OBJECT_STATE_VICTORY)
//     {
//         for (auto const &tile : interactiveTiles)
//         {
//             if (player1->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(player1, tile);
//         }

//         for (auto const &item : map.GetInteractiveItems())
//         {
//             if (player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(player1, item.get());
//         }

//         for (Enemy* enemy : map.GetEnemies())
//         {
//             if (player1->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(player1, enemy);
//         }
//     }
    
//     for (auto& fireball : *player1->GetFireballs())
//     {
//         for (auto const &tile : interactiveTiles)
//         {
//             if (fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(fireball, tile);
//         }

//         // Fireball với enemy
//         for (Enemy* enemy : map.GetEnemies())
//         {
//             if (enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(enemy, fireball);
//         }
//     }

//         // Xóa Enemy bị tiêu diệt
//     map.GetEnemies().erase(
//         std::remove_if(
//             map.GetEnemies().begin(),
//             map.GetEnemies().end(),
//             [](Enemy* enemy){
//                 return (enemy->GetState() == OBJECT_STATE_DEAD || enemy->GetState() == OBJECT_STATE_TO_BE_REMOVED);
//             }
//         ),
//         map.GetEnemies().end()
//     );
//     // Xóa Enemy bị tiêu diệt
//     // 8. Cập nhật và xử lý enemy
//     for (Enemy* enemy : map.GetEnemies())
//     {
//         enemy->UpdateStateAndPhysic();

//         // Enemy với tile
//         for (auto const& tile : interactiveTiles)
//         {
//             if (enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(enemy, tile);
//         }
//     }

//     // 7. Cập nhật và xử lý item
//     for (auto  &item : map.GetInteractiveItems())
//     {
//         item->Update();

//         for (auto const &tile : interactiveTiles)
//         {
//             if (item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
//                 mediatorCollision.HandleCollision(item.get(), tile);
//         }
//     }

//     for (auto &block : map.getBlocks())
// {
//     block->Update();    
//     CollisionType collision = block->checkCollisionType(*player1);
//     if( collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED)
//     {   
//         if(!block->isHit()) player1->SetVel(Vector2{player1->GetVel().x, 0});
//         block->doHit(*player1, this->GetMap());
//     }
//     if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION)
//     {   
//         block->doHit(*player1, this->GetMap());
//         player1->SetVel({player1->GetVel().x, 0});
//     }
//     if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS)
//     {
//         block->doHit(*player1, this->GetMap());
//         player1->SetVel({player1->GetVel().x, 0});
//     }
    
//     mediatorCollision.HandleCollision(player1, block);
// }   
//     // Xóa Blocks đã bị phá vỡ 
//     auto& blocks = map.getBlocks();
//         blocks.erase(
//             std::remove_if(
//                 blocks.begin(),
//                 blocks.end(),
//                 [](Block* block) {
//                     return block->GetState() == OBJECT_STATE_TO_BE_REMOVED;
//                 }
//             ),
//             blocks.end()
//         );
//     // Xóa Blocks đã bị phá vỡ 


//     // Xóa Item đã bị ăn 
//     auto &interactiveItems = map.GetInteractiveItems();
//     interactiveItems.erase(
//         std::remove_if(
//             interactiveItems.begin(),
//             interactiveItems.end(),
//             [](const std::shared_ptr<Item>& item) {
//                 return item->GetState() == OBJECT_STATE_TO_BE_REMOVED || item->GetState() == OBJECT_STATE_DEAD;
//             }
//         ),
//         interactiveItems.end()  
//     );
//     //Xóa ITem đã bị ăn 
//     if (player1->GetState() == OBJECT_STATE_DEAD) { // ← Sửa từ TO_BE_REMOVED thành DEAD
//         if (player1->GetLives() > 0) {
//             gameState = GameState::GAME_RESET;
//         } else {
//             gameState = GameState::GAME_OVER;
//         }
//     }

//     if (player1->GetState() == OBJECT_STATE_VICTORY) {
//         gameState = GameState::GAME_COMPLETED;
//         return;
//     }

// }

void GameWorld::UpdateWorld()
{
    // Update Player1 1
    if (player1) {
        player1->UpdateStateAndPhysic();
        player1->UpdateCollisionProbes();
        
        if (player1->GetState() != OBJECT_STATE_DEAD &&
            player1->GetState() != OBJECT_STATE_DYING &&
            player1->GetState() != OBJECT_STATE_VICTORY)
        {
            // Handle Player1 1 collisions
            for (auto const &tile : interactiveTiles)
            {
                if (player1->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, tile);
            }

            for (auto const &item : map.GetInteractiveItems())
            {
                if (player1->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, item.get());
            }

            for (Enemy* enemy : map.GetEnemies())
            {
                if (player1->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player1, enemy);
            }
        }
        
        // Handle Player1 1 fireballs
        for (auto& fireball : *player1->GetFireballs())
        {
            for (auto const &tile : interactiveTiles)
            {
                if (fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(fireball, tile);
            }

            for (Enemy* enemy : map.GetEnemies())
            {
                if (enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(enemy, fireball);
            }
        }
    }

    // Update Player1 2 (if multiplayer1)
    if (isMultiplayer && player2) {
        player2->UpdateStateAndPhysic();
        player2->UpdateCollisionProbes();
        
        if (player2->GetState() != OBJECT_STATE_DEAD &&
            player2->GetState() != OBJECT_STATE_DYING &&
            player2->GetState() != OBJECT_STATE_VICTORY)
        {
            // Handle Player1 2 collisions
            for (auto const &tile : interactiveTiles)
            {
                if (player2->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, tile);
            }

            for (auto const &item : map.GetInteractiveItems())
            {
                if (player2->checkCollisionType(*item) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, item.get());
            }

            for (Enemy* enemy : map.GetEnemies())
            {
                if (player2->checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(player2, enemy);
            }
        }
        
        // Handle Player1 2 fireballs
        for (auto& fireball : *player2->GetFireballs())
        {
            for (auto const &tile : interactiveTiles)
            {
                if (fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(fireball, tile);
            }

            for (Enemy* enemy : map.GetEnemies())
            {
                if (enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                    mediatorCollision.HandleCollision(enemy, fireball);
            }
        }
    }

    // ...existing enemy cleanup code...
    map.GetEnemies().erase(
        std::remove_if(
            map.GetEnemies().begin(),
            map.GetEnemies().end(),
            [](Enemy* enemy){
                return (enemy->GetState() == OBJECT_STATE_DEAD || enemy->GetState() == OBJECT_STATE_TO_BE_REMOVED);
            }
        ),
        map.GetEnemies().end()
    );

    // ...existing enemy and item update code...
    for (Enemy* enemy : map.GetEnemies())
    {
        enemy->UpdateStateAndPhysic();

        for (auto const& tile : interactiveTiles)
        {
            if (enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(enemy, tile);
        }
    }

    for (auto &item : map.GetInteractiveItems())
    {
        item->Update();

        for (auto const &tile : interactiveTiles)
        {
            if (item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(item.get(), tile);
        }
    }

    // Handle blocks for both player1s
    for (auto &block : map.getBlocks())
    {
        block->Update();    
        
        // Player1 1 block collisions
        if (player1) {
            CollisionType collision = block->checkCollisionType(*player1);
            if( collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED)
            {   
                if(!block->isHit()) player1->SetVel(Vector2{player1->GetVel().x, 0});
                block->doHit(*player1, this->GetMap());
            }
            if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION)
            {   
                block->doHit(*player1, this->GetMap());
                player1->SetVel({player1->GetVel().x, 0});
            }
            if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS)
            {
                block->doHit(*player1, this->GetMap());
                player1->SetVel({player1->GetVel().x, 0});
            }
            mediatorCollision.HandleCollision(player1, block);
        }
        
        // Player1 2 block collisions
        if (isMultiplayer && player2) {
            CollisionType collision2 = block->checkCollisionType(*player2);
            if( collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED)
            {   
                if(!block->isHit()) player2->SetVel(Vector2{player2->GetVel().x, 0});
                block->doHit(*player2, this->GetMap());
            }
            if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION)
            {   
                block->doHit(*player2, this->GetMap());
                player2->SetVel({player2->GetVel().x, 0});
            }
            if (collision2 == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS)
            {
                block->doHit(*player2, this->GetMap());
                player2->SetVel({player2->GetVel().x, 0});
            }
            mediatorCollision.HandleCollision(player2, block);
        }
    }
    
    // ...existing cleanup code...
    auto& blocks = map.getBlocks();
    blocks.erase(
        std::remove_if(
            blocks.begin(),
            blocks.end(),
            [](Block* block) {
                return block->GetState() == OBJECT_STATE_TO_BE_REMOVED;
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
                return item->GetState() == OBJECT_STATE_TO_BE_REMOVED || item->GetState() == OBJECT_STATE_DEAD;
            }
        ),
        interactiveItems.end()  
    );

    // Game state logic for multiplayer1
    bool player1Dead = (player1 && player1->GetState() == OBJECT_STATE_DEAD);
    bool player2Dead = (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_DEAD);
    
    if (isMultiplayer) {
        // In multiplayer1, game over only if both player1s are dead
        if (player1Dead && player2Dead) {
            if ((player1 && player1->GetLives() > 0) || (player2 && player2->GetLives() > 0)) {
                gameState = GameState::GAME_RESET;
            } else {
                gameState = GameState::GAME_OVER;
            }
        }
    } else {
        // Single player1 logic
        if (player1Dead) {
            if (player1 && player1->GetLives() > 0) {
                gameState = GameState::GAME_RESET;
            } else {
                gameState = GameState::GAME_OVER;
            }
        }
    }

    // Victory condition - any player1 reaches the end
    if ((player1 && player1->GetState() == OBJECT_STATE_VICTORY) || 
        (isMultiplayer && player2 && player2->GetState() == OBJECT_STATE_VICTORY)) {
        gameState = GameState::GAME_COMPLETED;
        return;
    }
}

// void GameWorld::DrawWorld()
// {
//     camera.target.y = GetScreenHeight() / 2;
//     if (player1->GetPos().x > GetScreenWidth() / 2 && player1->GetPos().x < map.GetWidth() - GetScreenWidth() / 2)
//     {
//         camera.target.x = player1->GetPos().x;
//     }
//     else if (player1->GetPos().x <= GetScreenWidth() / 2)
//     {
//         camera.target.x = GetScreenWidth() / 2;
//     }
//     else
//     {
//         camera.target.x = map.GetWidth() - GetScreenWidth() / 2;
//     }

//     if (camera.target.x - GetScreenWidth() / 2 >= BGpos)
//     {
//         BGpos = BGpos + background.width * 1.3f;
//     }
//     if (camera.target.x + GetScreenWidth() / 2 <= BGpos + background.width * 1.3f)
//     {
//         BGpos = BGpos - background.width * 1.3f;
//     }

//     BeginMode2D(camera);
//     DrawTextureEx(background, Vector2{BGpos - background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
//     DrawTextureEx(background, Vector2{BGpos, -200}, 0.0f, 1.3f, WHITE);
//     DrawTextureEx(background, Vector2{BGpos + background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
//     map.Draw();
//     player1->Draw();
//     // std::cout << "Player1 Position: " << player1->GetPos().x << ", " << player1->GetPos().y << std::endl;

//     // for (auto const &item : interactiveItems)
//     // {
//     //     item->Draw();
//     // }
//     EndMode2D();
// }

void GameWorld::DrawWorld()
{
    // Camera follows the active player1 (or average position in multiplayer1)
    if (isMultiplayer && player2) {
        // Follow average position of both player1s
        float avgX = (player1->GetPos().x + player2->GetPos().x) / 2;
        camera.target.y = GetScreenHeight() / 2;
        
        if (avgX > GetScreenWidth() / 2 && avgX < map.GetWidth() - GetScreenWidth() / 2)
        {
            camera.target.x = avgX;
        }
        else if (avgX <= GetScreenWidth() / 2)
        {
            camera.target.x = GetScreenWidth() / 2;
        }
        else
        {
            camera.target.x = map.GetWidth() - GetScreenWidth() / 2;
        }
    } else {
        // Single player1 camera
        camera.target.y = GetScreenHeight() / 2;
        if (player1->GetPos().x > GetScreenWidth() / 2 && player1->GetPos().x < map.GetWidth() - GetScreenWidth() / 2)
        {
            camera.target.x = player1->GetPos().x;
        }
        else if (player1->GetPos().x <= GetScreenWidth() / 2)
        {
            camera.target.x = GetScreenWidth() / 2;
        }
        else
        {
            camera.target.x = map.GetWidth() - GetScreenWidth() / 2;
        }
    }

    if (camera.target.x - GetScreenWidth() / 2 >= BGpos)
    {
        BGpos = BGpos + background.width * 1.3f;
    }
    if (camera.target.x + GetScreenWidth() / 2 <= BGpos + background.width * 1.3f)
    {
        BGpos = BGpos - background.width * 1.3f;
    }

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{BGpos - background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{BGpos + background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    map.Draw();
    
    // Draw both player1s
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