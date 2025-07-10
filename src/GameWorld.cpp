#include "GameWorld.h"

//=============================
//Nhi
GameWorld::GameWorld()
    : player(),
      interactiveTiles(map.getInteractiveTiles()) // nếu vẫn dùng tile riêng
{
    // Thiết lập Mario
    player = Mario(Vector2{100, 100}, 3, SMALL);

    // Load bản đồ
    map.LoadMap(0);

    // Thêm tất cả item vào danh sách đa hình chung
    for (auto& coin : map.getInteractiveCoins()) {
        interactiveItems.push_back(coin);
    }

    for (auto& token : map.getInteractiveCourseClearTokens()) {
        interactiveItems.push_back(token);
    }

    for (auto& flower : map.getInteractiveFireFlowers()) {
        interactiveItems.push_back(flower);
    }

    for (auto& mushroom : map.getInteractiveMushrooms()) {
        interactiveItems.push_back(mushroom);
    }

    for (auto& oneUp : map.getInteractiveOneUpMushrooms()) {
        interactiveItems.push_back(oneUp);
    }

    for (auto& star : map.getInteractiveStarts()) {
        interactiveItems.push_back(star);
    }

    for (auto& moon : map.getInteractiveThreeUpMoons()) {
        interactiveItems.push_back(moon);
    }

    for (auto& yoshi : map.getInteractiveYoshiCoins()) {
        interactiveItems.push_back(yoshi);
    }

    // Cài đặt camera
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player.GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

//=======================



GameWorld::~GameWorld()
{
    // Destructor logic if needed
}

void GameWorld::UpdateWorld()
{
    player.UpdateStateAndPhysic();
    for ( auto const &tile : interactiveTiles )
    {
        CollisionType collision = player.checkCollisionType(*tile);
        if ( collision )
        {
            mediatorCollision.HandleCollision(&player, tile);
        }

        for ( auto &fireball : *player.GetFireballs() )
        {
            CollisionType fireballCollision = fireball->checkCollisionType(*tile);
            if ( fireballCollision  )
            {
                mediatorCollision.HandleCollision(fireball, tile);
            }
        }
    }
//=================================
//Nhi
    for (auto const& item : interactiveItems) {
        CollisionType collision = player.checkCollisionType(*item);
        if (collision) {
            mediatorCollision.HandleCollision(&player, item.get());
        }
        item->Update(); // animation hoặc xoay tùy loại
    }
//================

    
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

    if ( camera.target.x - GetScreenWidth() / 2 >= currBackgroundStarX )
    {
        currBackgroundStarX = currBackgroundStarX + background.width * 1.3f;
    }
    if ( camera.target.x + GetScreenWidth() / 2 <= currBackgroundStarX + background.width * 1.3f )
    {
        currBackgroundStarX = currBackgroundStarX - background.width * 1.3f;
    }

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{currBackgroundStarX-background.width*1.3f,-200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background,Vector2{currBackgroundStarX,-200},0.0f,1.3f,WHITE);
    DrawTextureEx(background,Vector2{currBackgroundStarX+background.width*1.3f,-200},0.0f,1.3f,WHITE);
    map.Draw();
    player.Draw();
    //========================
    //Nhi
    for (auto const& item : interactiveItems) {
        item->Draw();
    }
    //===========================

    EndMode2D();
}

const float GameWorld::GetGravity()
{
    return gravity;
}

void GameWorld::Init()
{
    ResrcManager::GetInstance().loadResources();
}
