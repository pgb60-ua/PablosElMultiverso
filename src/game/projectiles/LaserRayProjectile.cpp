#include "LaserRayProjectile.hpp"
#include "SpriteSheet.hpp"
#include "SpriteLoaderManager.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>

LaserRayProjectile::LaserRayProjectile(std::vector<AEnemy *> &allEnemies)
    : AProjectile(allEnemies)
{
    // Calcular el radio basado en el sprite del proyectil Laser Ray
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
    if (!sheet.frames.empty())
    {
        const Rectangle &frame = sheet.frames[0];
        // El radio es la mitad del lado más pequeño del sprite
        float width = std::abs(frame.width);
        float height = std::abs(frame.height);
        float radius = std::min(width, height) * 0.5f;
        setRadius(radius);
    }
}

void LaserRayProjectile::update(float deltaTime)
{
    if (!active)
        return;

    Vector2 position = getShapePosition(shape);
    position.x += direction.x * stats.GetMovementSpeed() * deltaTime;
    position.y += direction.y * stats.GetMovementSpeed() * deltaTime;
    setShapePosition(shape, position);

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }

    for (auto &enemy : enemiesInScene)
    {
        if (enemy->IsAlive() && checkCollisionShapes(shape, enemy->GetHitbox()))
        {
            enemy->TakeDamage(stats);
            if (!enemy->IsAlive())
            {
                delete enemy;
                enemiesInScene.erase(std::find(enemiesInScene.begin(), enemiesInScene.end(), enemy));
            }
            deactivate();
            break;
        }
    }
    timeAlive -= deltaTime;
    if (timeAlive <= 0.0f)
    {
        deactivate();
    }
}

LaserRayProjectile::~LaserRayProjectile()
{
}

void LaserRayProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width * 0.5f, src.height * 0.5f};

    Rectangle dest = {getPosition().x, getPosition().y,
                      src.width, src.height};

    // Rotate according to getDirection()
    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
        angleDeg += 90.0f;
    }

    DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
}
