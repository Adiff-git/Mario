#include "GameWorld.h"
#include "Goomba.h"
#include "GreenKoopa.h"
#include "YellowKoopa.h"
#include "RedKoopa.h"
#include "FlyingGoomba.h"
#include "Piranhaplant.h"
#include "JumpingPiranha.h"
GameWorld::GameWorld() : player(), interactiveTiles(map.getInteractiveTiles())
{
    player = Mario(Vector2{100, 100}, 3, SMALL);
    map.LoadMap(0);
    camera.offset = Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2};
    camera.target = player.GetPos();
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // mediatorCollision.GetEnemies().push_back(new Goomba(Vector2{200, 800}));
    // mediatorCollision.GetEnemies().push_back(new GreenKoopa(Vector2{300, 790}));
    // mediatorCollision.GetEnemies().push_back(new YellowKoopa(Vector2{400, 800}));
    // mediatorCollision.GetEnemies().push_back(new RedKoopa(Vector2{400, 800}));
    // mediatorCollision.GetEnemies().push_back(new FlyingGoomba(Vector2{400, 812}));
    // mediatorCollision.GetEnemies().push_back(new PiranhaPlant(Vector2{500, 816}));
    // mediatorCollision.GetEnemies().push_back(new JumpingPiranhaPlant(Vector2{600, 816}));
}

GameWorld::~GameWorld()
{
    for (auto tile : interactiveTiles)
        delete tile;
    for (auto enemy : mediatorCollision.GetEnemies())
        delete enemy;
}
//==============================
// nhi
void GameWorld::UpdateWorld()
{
    player.UpdateStateAndPhysic();

    for (Enemy *enemy : mediatorCollision.GetEnemies())
    {
        enemy->Update();
    }

    for (auto const &tile : interactiveTiles)
    {
        CollisionType collision = player.checkCollisionType(*tile);
        if (collision)
            mediatorCollision.HandleCollision(&player, tile);

        for (auto &fireball : *player.GetFireballs())
        {
            CollisionType fireballCollision = fireball->checkCollisionType(*tile);
            if (fireballCollision)
                mediatorCollision.HandleCollision(fireball, tile);
        }

        for (Enemy *enemy : mediatorCollision.GetEnemies())
        {
            mediatorCollision.HandleCollision(enemy, tile);
        }
    }

    for (Enemy *enemy : mediatorCollision.GetEnemies())
    {
        CollisionType marioEnemyCollision = player.checkCollisionType(*enemy);
        if (marioEnemyCollision)
            mediatorCollision.HandleCollision(&player, enemy);
    }

    for (Enemy *enemy : mediatorCollision.GetEnemies())
    {
        for (auto &fireball : *player.GetFireballs())
        {
            CollisionType enemyFireballCollision = enemy->checkCollisionType(*fireball);
            if (enemyFireballCollision)
                mediatorCollision.HandleCollision(enemy, fireball);
        }
    }
}
//================================

void GameWorld::DrawWorld()
{
    camera.target.y = GetScreenHeight() / 2;
    if (player.GetPos().x > GetScreenWidth() / 2 && player.GetPos().x < map.GetWidth() - GetScreenWidth() / 2)
    {
        camera.target.x = player.GetPos().x;
    }
    else if (player.GetPos().x <= GetScreenWidth() / 2)
    {
        camera.target.x = GetScreenWidth() / 2;
    }
    else
    {
        camera.target.x = map.GetWidth() - GetScreenWidth() / 2;
    }

    if (camera.target.x - GetScreenWidth() / 2 >= currBackgroundStarX)
    {
        currBackgroundStarX = currBackgroundStarX + background.width * 1.3f;
    }
    if (camera.target.x + GetScreenWidth() / 2 <= currBackgroundStarX + background.width * 1.3f)
    {
        currBackgroundStarX = currBackgroundStarX - background.width * 1.3f;
    }

    BeginMode2D(camera);
    DrawTextureEx(background, Vector2{currBackgroundStarX - background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{currBackgroundStarX, -200}, 0.0f, 1.3f, WHITE);
    DrawTextureEx(background, Vector2{currBackgroundStarX + background.width * 1.3f, -200}, 0.0f, 1.3f, WHITE);
    map.draw();
    player.draw();

    for (Enemy *enemy : mediatorCollision.GetEnemies())
    {
        enemy->draw();
    }
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