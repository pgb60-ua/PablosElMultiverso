#include "EggplosiveProjectile.hpp"
#include "SpriteLoaderManager.hpp"
#include <cmath>
#include <algorithm>

EggplosiveProjectile::EggplosiveProjectile(std::vector<AEnemy *> &allEnemies)
    : AProjectile(allEnemies)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::SNIPER);
    if (!sheet.frames.empty())
    {
        const Rectangle &frame = sheet.frames[0];
        float width = std::abs(frame.width);
        float height = std::abs(frame.height);
        float radius = std::min(width, height) * 0.5f;
        setRadius(radius);
    }
}

EggplosiveProjectile::~EggplosiveProjectile()
{
}

void EggplosiveProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats)
{
    AProjectile::activate(position, direction, stats);
    timeAlive = TIME_TO_BE_ALIVE;
    hasExploded = false;
}

void EggplosiveProjectile::update(float deltaTime)
{
    if (!active)
        return;

    if (hasExploded)
    {
        Explode(deltaTime);
        timeAlive -= deltaTime;
        if (timeAlive <= 0.0f)
        {
            deactivate();
        }
        return;
    }

    Vector2 position = getPosition();
    position.x += direction.x * stats.GetMovementSpeed() * deltaTime;
    position.y += direction.y * stats.GetMovementSpeed() * deltaTime;
    setShapePosition(shape, position);

    for (auto &enemy : enemiesInScene)
    {
        if (enemy->IsAlive() && checkCollisionShapes(shape, enemy->GetHitbox()))
        {
            hasExploded = true;
            timeAlive = TIME_TO_BE_ALIVE;
            return;
        }
    }
}

void EggplosiveProjectile::Explode(float deltaTime)
{
    Vector2 explosionCenter = getPosition();
    
    for (auto &enemy : enemiesInScene)
    {
        if (!enemy->IsAlive())
            continue;
            
        Shape enemyHitbox = enemy->GetHitbox();
        Vector2 enemyCenter = {
            enemyHitbox.data.rectangle.x + enemyHitbox.data.rectangle.width * 0.5f,
            enemyHitbox.data.rectangle.y + enemyHitbox.data.rectangle.height * 0.5f
        };
        
        float dx = enemyCenter.x - explosionCenter.x;
        float dy = enemyCenter.y - explosionCenter.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance <= EXPLOSION_RADIUS)
        {
            Stats copia = getStats();
            copia.SetMagicDamage(copia.GetMagicDamage() * deltaTime); 
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

void EggplosiveProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::SNIPER);
    if (sheet.frames.empty())
        return;
        
    animation.frameIndex %= sheet.spriteFrameCount;
    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {
        src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
        src.height > 0 ? src.height * 0.5f : -src.height * 0.5f
    };

    Rectangle dest = {
        getPosition().x, 
        getPosition().y,
        src.width, 
        src.height
    };

    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
    }

    if (!hasExploded)
    {
        DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
    }
    else
    {
        float alpha = (timeAlive / TIME_TO_BE_ALIVE) * 150.0f; // Fade out
        Color poisonColor = {0, 255, 0, (unsigned char)alpha};
        DrawCircle(getPosition().x, getPosition().y, EXPLOSION_RADIUS, poisonColor);
        DrawCircleLines(getPosition().x, getPosition().y, EXPLOSION_RADIUS, GREEN);
    }
}
