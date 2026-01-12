#include "Zombie.hpp"
#include "ScreenConstants.hpp"
#include <cmath>
#include <raymath.h>
#include "AudioManager.hpp"

Zombie::Zombie(std::vector<Player *> objectives)
    : AEnemy(DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::ZOMBIE),
             SpriteLoaderManager::GetInstance().GetSpriteHitbox(
                 ENEMY_TYPE::ZOMBIE,
                 Vector2{(float)(std::rand() % ENEMY_SCREEN_WIDTH), (float)(std::rand() % ENEMY_SCREEN_HEIGHT)}),
             objectives, 15)
{
    // Las stats se cargan automáticamente desde zombie.json en la lista de inicialización
}

void Zombie::Render()
{
    if (!IsAlive())
        return;
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width * 0.5f, src.height * 0.5f};

    Rectangle dest = {hitbox.data.rectangle.x + hitbox.data.rectangle.width * 0.5f,
                      hitbox.data.rectangle.y + hitbox.data.rectangle.height * 0.5f, src.width, src.height};

    DrawTexturePro(sheet.texture, src, dest, origin, 0, animation.color);
    animation.color = WHITE;
}

void Zombie::Update(float deltaTime)
{
    // Actualiza el cooldown de ataque
    currentAttackCooldownTime += deltaTime;

    Move(deltaTime);

    // Regeneración de vida
    if (stats.GetHealthRegeneration() > 0 && IsAlive())
    {
        float newHealth = stats.GetHealth() + (stats.GetHealthRegeneration() * deltaTime);
        if (newHealth > stats.GetMaxHealth())
            newHealth = stats.GetMaxHealth();
        stats.SetHealth(newHealth);
    }
    UpdateEnemyAnimation(deltaTime, ENEMY_TYPE::ZOMBIE);
}
