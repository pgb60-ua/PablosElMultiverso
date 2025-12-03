#include "EggplosiveProjectile.hpp"
#include "SpriteLoaderManager.hpp"
#include <cmath>
#include <algorithm>

EggplosiveProjectile::EggplosiveProjectile(std::vector<AEnemy *> &allEnemies)
    : AProjectile(allEnemies)
{
    // Configurar hitbox circular para el proyectil
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

    // Si ya explotó, aplicar daño continuo
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

    // Mover el proyectil
    Vector2 position = getPosition();
    position.x += direction.x * stats.GetMovementSpeed() * deltaTime;
    position.y += direction.y * stats.GetMovementSpeed() * deltaTime;
    setShapePosition(shape, position);

    // Verificar colisión con enemigos
    for (auto &enemy : enemiesInScene)
    {
        if (enemy->IsAlive() && checkCollisionShapes(shape, enemy->GetHitbox()))
        {
            // Explotar al tocar un enemigo
            hasExploded = true;
            timeAlive = TIME_TO_BE_ALIVE; // Reiniciar el tiempo para el veneno
            return;
        }
    }
}

void EggplosiveProjectile::Explode(float deltaTime)
{
    Vector2 explosionCenter = getPosition();
    
    // Causar daño a todos los enemigos dentro del radio de explosión
    for (auto &enemy : enemiesInScene)
    {
        if (!enemy->IsAlive())
            continue;
            
        // Calcular la posición del centro del enemigo
        Shape enemyHitbox = enemy->GetHitbox();
        Vector2 enemyCenter = {
            enemyHitbox.data.rectangle.x + enemyHitbox.data.rectangle.width * 0.5f,
            enemyHitbox.data.rectangle.y + enemyHitbox.data.rectangle.height * 0.5f
        };
        
        // Calcular distancia desde el centro de la explosión
        float dx = enemyCenter.x - explosionCenter.x;
        float dy = enemyCenter.y - explosionCenter.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        
        // Si el enemigo está dentro del radio, causar daño
        if (distance <= EXPLOSION_RADIUS)
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

    // Calcular rotación según la dirección
    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
    }

    // Solo dibujar el sprite si no ha explotado
    if (!hasExploded)
    {
        DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
    }
    else
    {
        // Dibujar charco de veneno
        float alpha = (timeAlive / TIME_TO_BE_ALIVE) * 150.0f; // Fade out
        Color poisonColor = {0, 255, 0, (unsigned char)alpha};
        DrawCircle(getPosition().x, getPosition().y, EXPLOSION_RADIUS, poisonColor);
        DrawCircleLines(getPosition().x, getPosition().y, EXPLOSION_RADIUS, GREEN);
    }
}
