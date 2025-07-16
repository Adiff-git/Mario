#include "GameWorld.h"
#include "Coin.h"
#include "CourseClearToken.h"
#include "FireFlower.h"
#include "Mushroom.h"
#include "OneUpMushroom.h"
#include "Star.h"
#include "ThreeUpMoon.h"
#include "YoshiCoin.h"

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
        interactiveItems.push_back(std::make_shared<Coin>(Vector2{150, 500}));
        interactiveItems.push_back(std::make_shared<CourseClearToken>(Vector2{150, 500}));
        interactiveItems.push_back(std::make_shared<FireFlower>(Vector2{200, 500}));
        interactiveItems.push_back(std::make_shared<Mushroom>(Vector2{250, 500}));
        interactiveItems.push_back(std::make_shared<OneUpMushroom>(Vector2{300, 500}));
        interactiveItems.push_back(std::make_shared<Star>(Vector2{350, 500}));
        interactiveItems.push_back(std::make_shared<ThreeUpMoon>(Vector2{400, 500}));
        interactiveItems.push_back(std::make_shared<YoshiCoin>(Vector2{450, 500}));
    }
}


GameWorld::~GameWorld()
{
    // Destructor logic if needed
}

void GameWorld::UpdateWorld()
{
    // Cập nhật trạng thái vật lý Mario
    player.UpdateStateAndPhysic();

    // Kiểm tra nếu Mario còn sống hoặc chưa thắng
    if (player.GetState() == OBJECT_STATE_DEAD ||
        player.GetState() == OBJECT_STATE_DYING ||
        player.GetState() == OBJECT_STATE_VICTORY)
        return;

    // ================= TILE COLLISIONS ==================
    for (auto const& tile : interactiveTiles)
    {
        // Player ↔ Tile
        CollisionType marioTileCollision = player.checkCollisionType(*tile);
        if (marioTileCollision)
            mediatorCollision.HandleCollision(&player, tile);

        // Fireball ↔ Tile
        for (auto& fireball : *player.GetFireballs())
        {
            CollisionType fireballCollision = fireball->checkCollisionType(*tile);
            if (fireballCollision)
                mediatorCollision.HandleCollision(fireball, tile);
        }

        // Enemy ↔ Tile
        // for (Enemy* enemy : mediatorCollision.GetEnemies())
        // {
        //     CollisionType enemyCollision = enemy->checkCollisionType(*tile);
        //     if (enemyCollision)
        //         mediatorCollision.HandleCollision(enemy, tile);
        // }

        // Item ↔ Tile
        for (auto& item : interactiveItems)
        {
            CollisionType itemTileCollision = item->checkCollisionType(*tile);
            if (itemTileCollision)
                mediatorCollision.HandleCollision(item.get(), tile);
        }
    }

    // ================= ENTITY COLLISIONS ==================

    // Player ↔ Enemy
    // for (Enemy* enemy : mediatorCollision.GetEnemies())
    // {
    //     CollisionType marioEnemyCollision = player.checkCollisionType(*enemy);
    //     if (marioEnemyCollision)
    //         mediatorCollision.HandleCollision(&player, enemy);
    // }

    // // Fireball ↔ Enemy
    // for (Enemy* enemy : mediatorCollision.GetEnemies())
    // {
    //     for (auto& fireball : *player.GetFireballs())
    //     {
    //         CollisionType enemyFireballCollision = enemy->checkCollisionType(*fireball);
    //         if (enemyFireballCollision)
    //             mediatorCollision.HandleCollision(enemy, fireball);
    //     }
    // }

    // Player ↔ Item + Update Item
    for (auto& item : interactiveItems)
    {
        CollisionType playerItemCollision = player.checkCollisionType(*item);
        if (playerItemCollision)
            mediatorCollision.HandleCollision(&player, item.get());

        item->Update();
    }

    // Cập nhật enemy
    // for (Enemy* enemy : mediatorCollision.GetEnemies())
    // {
    //     enemy->UpdateStateAndPhysic();
    // }

    // Gỡ bỏ item đã được thu thập hoặc tiêu diệt
    interactiveItems.erase(
        std::remove_if(interactiveItems.begin(), interactiveItems.end(),
            [](const std::shared_ptr<Item>& item) {
                return item->GetState() == OBJECT_STATE_TO_BE_REMOVED ||
                       item->GetState() == OBJECT_STATE_DEAD;
            }),
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