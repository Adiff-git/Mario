#include "GameWorld.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "Star.h"
#include "ThreeUpMoon.h"
#include "YoshiCoin.h"
#include "EnemyManager.h"
GameWorld::GameWorld() : player(), 
interactiveTiles(map.getInteractiveTiles())
{   
    // Trong GameWorld constructor, thêm:

    player = Mario(Vector2{100, 100}, 3, SMALL); // Đặt vị trí cụ thể
    map.LoadMap(0);
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player.GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    background = ResrcManager::GetInstance().getTexture("BACKGROUND_0"); 
    
}

GameWorld::GameWorld(int MapID, GameScreen* gameScreen) : 
player(), 
interactiveTiles(map.getInteractiveTiles()), 
gameScreen(gameScreen),
gameState(GameState::GAME_PLAYING)
{
    map.LoadMap(MapID);
    player = Mario(Vector2{100, 100}, 3, SMALL); // Đặt vị trí cụ thể
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player.GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    switch (MapID) {
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
    if (MapID == 0) {
        interactiveItems.push_back(std::make_shared<Coin>(Vector2{150, 800}));
        interactiveItems.push_back(std::make_shared<CourseClearToken>(Vector2{150, 500}));
        interactiveItems.push_back(std::make_shared<FireFlower>(Vector2{200, 800}));
        interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{250, 500}));
        interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{300, 500}));
        interactiveItems.push_back(std::make_shared<Star>(Vector2{350, 500}));
        interactiveItems.push_back(std::make_shared<ThreeUpMoon>(Vector2{400, 500}));
        interactiveItems.push_back(std::make_shared<YoshiCoin>(Vector2{450, 500}));

        mediatorCollision.GetEnemies().push_back(new Goomba(Vector2{450, 500}));
        mediatorCollision.GetEnemies().push_back(new GreenKoopa(Vector2{500, 500}));
        mediatorCollision.GetEnemies().push_back(new BuzzyBeetle(Vector2{600, 500}));
        mediatorCollision.GetEnemies().push_back(new Rex(Vector2{650, 500}));
        mediatorCollision.GetEnemies().push_back(new FlyingGoomba(Vector2{750, 500}));
    }
}


GameWorld::~GameWorld()
{
    // Destructor logic if needed
}

// void GameWorld::UpdateWorld()
// {
//     player.UpdateStateAndPhysic();
//     const float ActiveWidth = GetScreenWidth() / 2.0f + 50;
//     if (player.GetState() != OBJECT_STATE_DEAD && 
//         player.GetState() != OBJECT_STATE_DYING &&
//         player.GetState() != OBJECT_STATE_VICTORY) {
//             for ( auto const &tile : interactiveTiles )
//             {
//                 CollisionType collision = player.checkCollisionType(*tile);
//                 if ( collision )
//                 {
//                     mediatorCollision.HandleCollision(&player, tile);
//                 }
        
//                 for ( auto &fireball : *player.GetFireballs() )
//                 {
//                     CollisionType fireballCollision = fireball->checkCollisionType(*tile);
//                     if ( fireballCollision  )
//                     {
//                         mediatorCollision.HandleCollision(fireball, tile);
//                     }
//                 }
//             }
        
//             for (auto const& item : interactiveItems) {
//                 CollisionType collision = player.checkCollisionType(*item);
//                 if (collision) {
//                     mediatorCollision.HandleCollision(&player, item.get());
//                 }
        
//                 item->Update();
//             }
//         }
//         for (Enemy *enemy : mediatorCollision.GetEnemies())
//     {
//         enemy->Update();
//     }

//     for (auto const &tile : interactiveTiles)
//     {
//         CollisionType collision = player.checkCollisionType(*tile);
//         if (collision)
//             mediatorCollision.HandleCollision(&player, tile);

//         for (auto &fireball : *player.GetFireballs())
//         {
//             CollisionType fireballCollision = fireball->checkCollisionType(*tile);
//             if (fireballCollision)
//                 mediatorCollision.HandleCollision(fireball, tile);
//         }

//         for (Enemy *enemy : mediatorCollision.GetEnemies())
//         {
//             mediatorCollision.HandleCollision(enemy, tile);
//         }
//     }

//     for (Enemy *enemy : mediatorCollision.GetEnemies())
//     {
//         CollisionType marioEnemyCollision = player.checkCollisionType(*enemy);
//         if (marioEnemyCollision)
//             mediatorCollision.HandleCollision(&player, enemy);
//     }

//     for (Enemy *enemy : mediatorCollision.GetEnemies())
//     {
//         for (auto &fireball : *player.GetFireballs())
//         {
//             CollisionType enemyFireballCollision = enemy->checkCollisionType(*fireball);
//             if (enemyFireballCollision)
//                 mediatorCollision.HandleCollision(enemy, fireball);
//         }
//     }
// }

void GameWorld::UpdateWorld()
{
    // 1. Cập nhật trạng thái và vật lý của người chơi
    player.UpdateStateAndPhysic();
    player.UpdateCollisionProbes();

    // 2. Kiểm tra nếu Mario vẫn đang chơi bình thường
    if (player.GetState() != OBJECT_STATE_DEAD &&
        player.GetState() != OBJECT_STATE_DYING &&
        player.GetState() != OBJECT_STATE_VICTORY)
    {
        // 3. Va chạm Mario với tile
        for (auto const& tile : interactiveTiles)
        {
            if (player.checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(&player, tile);
        }

        // 4. Va chạm Mario với item
        for (auto const& item : interactiveItems)
        {
            if (player.checkCollisionType(*item) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(&player, item.get());
        }

        // 5. Va chạm Mario với enemy
        for (Enemy* enemy : mediatorCollision.GetEnemies())
        {
            if (player.checkCollisionType(*enemy) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(&player, enemy);
        }
    }

    // 6. Cập nhật và xử lý fireball
    for (auto& fireball : *player.GetFireballs())
    {
        // Fireball với tile
        for (auto const& tile : interactiveTiles)
        {
            if (fireball->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(fireball, tile);
        }

        // Fireball với enemy
        for (Enemy* enemy : mediatorCollision.GetEnemies())
        {
            if (enemy->checkCollisionType(*fireball) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(enemy, fireball);
        }
    }

    // 7. Cập nhật và xử lý item
    for (auto const& item : interactiveItems)
    {
        item->Update();

        // Item với tile
        for (auto const& tile : interactiveTiles)
        {
            if (item->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(item.get(), tile);
        }
    }

    // 8. Cập nhật và xử lý enemy
    for (Enemy* enemy : mediatorCollision.GetEnemies())
    {
        enemy->Update();

        // Enemy với tile
        for (auto const& tile : interactiveTiles)
        {
            if (enemy->checkCollisionType(*tile) != COLLISION_TYPE_NONE)
                mediatorCollision.HandleCollision(enemy, tile);
        }
    }

    for (auto &block : map.getBlocks())
{
    block->Update();
    CollisionType collision = block->checkCollisionType(player);
    if( collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_EYES_OPENED)
    {   
        if(!block->isHit()) player.SetVel(Vector2{player.GetVel().x, 0});
        block->doHit(player, this);
    }
    if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_QUESTION)
    {   
        block->doHit(player, this);
        player.SetVel({player.GetVel().x, 0});
    }
    if (collision == COLLISION_TYPE_SOUTH && block->GetBlockType() == BLOCK_GLASS)
    {
        block->doHit(player, this);
        player.SetVel({player.GetVel().x, 0});
    }
    
    mediatorCollision.HandleCollision(&player, block);
}
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
    
}


void GameWorld::DrawWorld()
{
    camera.target.y = GetScreenHeight() / 2;
    if ( player.GetPos().x > GetScreenWidth() / 2 && player.GetPos().x < map.GetWidth() - GetScreenWidth() / 2) {
        camera.target.x = player.GetPos().x;
    } else if (player.GetPos().x <= GetScreenWidth() / 2) {
        camera.target.x = GetScreenWidth() / 2;
    } else {
        camera.target.x = map.GetWidth() - GetScreenWidth() / 2;
    } 

    if ( camera.target.x - GetScreenWidth() / 2 >= BGpos )
    {
        BGpos = BGpos + background.width * 1.3f;
    }
    if ( camera.target.x + GetScreenWidth() / 2 <= BGpos + background.width * 1.3f )
    {
        BGpos = BGpos - background.width * 1.3f;
    }

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{BGpos-background.width*1.3f,-200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background,Vector2{BGpos,-200},0.0f,1.3f,WHITE);
    DrawTextureEx(background,Vector2{BGpos+background.width*1.3f,-200},0.0f,1.3f,WHITE);
    map.Draw();
    player.Draw();
    std::cout << "Player Position: " << player.GetPos().x << ", " << player.GetPos().y << std::endl;

    for (auto const& item : interactiveItems) {
        item->Draw();
    }
    for (Enemy* enemy : mediatorCollision.GetEnemies()) {
        enemy->Draw();
    }
    EndMode2D();
}

bool GameWorld::IsCompleted()
{
    return gameState == GameState::GAME_COMPLETED ;
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

Map GameWorld::GetMap() {
    return map;
}