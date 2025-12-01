#include "LaserRayProjectile.hpp"
#include "SpriteSheet.hpp"
#include "SpriteLoaderManager.hpp"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <rlgl.h>

LaserRayProjectile::LaserRayProjectile(std::vector<AEnemy *> &allEnemies)
    : AProjectile(allEnemies)
{
    shape.type = ShapeType::SHAPE_ROTATED_RECTANGLE;
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
    if (!sheet.frames.empty())
    {
        const Rectangle &frame = sheet.frames[0];
        float width = std::abs(frame.width);
        float height = std::abs(frame.height);
        Vector2 pos = getPosition();
        shape.data.rotatedRectangle = {pos.x, pos.y, width, height, 0.0f};
    }
}

void LaserRayProjectile::update(float deltaTime)
{
    if (!active)
        return;
    for (auto &enemy : enemiesInScene)
    {
        if (!enemy->IsAlive())
            continue;

        // Usar la función checkCollisionShapes
        if (checkCollisionShapes(shape, enemy->GetHitbox()))
        {
            Stats copia = getStats();
            copia.SetMagicDamage(copia.GetMagicDamage() * deltaTime); // Daño por segundo
            enemy->TakeDamage(copia);
            if (!enemy->IsAlive())
            {
                delete enemy;
                enemiesInScene.erase(std::find(enemiesInScene.begin(), enemiesInScene.end(), enemy));
            }
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

void LaserRayProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats)
{
    AProjectile::activate(position, direction, stats);
    timeAlive = TIME_TO_BE_ALIVE;
}

void LaserRayProjectile::updatePositionAndDirection(Vector2 newPosition, Vector2 newDirection)
{
    setShapePosition(shape, newPosition);
    
    direction = newDirection;
    
    if (newDirection.x != 0.0f || newDirection.y != 0.0f)
    {
        shape.data.rotatedRectangle.rotation = atan2f(newDirection.y, newDirection.x) * RAD2DEG;
    }
}

void LaserRayProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};
    Rectangle dest = {getPosition().x, getPosition().y,
                      src.width, src.height};

    // Rotate according to getDirection()
    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
    }

    DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
}
