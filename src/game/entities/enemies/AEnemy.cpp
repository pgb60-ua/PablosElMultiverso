#include "AEnemy.hpp"
#include <cmath>
#include <limits>
#include <raymath.h>

AEnemy::AEnemy(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath)
    : AEntity(stats, hitbox), objectives(objectives), pabloCoinsAtDeath(pabloCoinsAtDeath)
{
}

void AEnemy::SetObjective(const std::vector<Player *> newObjectives) { objectives = newObjectives; }

void AEnemy::SetPabloCoinsAtDeath(int cantidad) { pabloCoinsAtDeath = cantidad; }

int AEnemy::GetPabloCoinsAtDeath() const { return pabloCoinsAtDeath; }

Player *AEnemy::GetClosestPlayer()
{
    if (objectives.empty())
        return nullptr;

    // Búsqueda del jugador más cercano
    float minDistance = std::numeric_limits<float>::max();
    int closestIndex = -1;

    for (size_t i = 0; i < objectives.size(); ++i)
    {
        Vector2 playerPos = objectives[i]->GetPosition();
        float distance = Vector2Distance(GetPosition(), playerPos);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestIndex = static_cast<int>(i);
        }
    }

    return objectives[closestIndex];
}

void AEnemy::UpdateEnemyAnimation(float deltaTime, ENEMY_TYPE enemyType)
{
    animation.timeAccumulator += deltaTime;

    if (animation.timeAccumulator >= animation.FRAME_DURATION)
    {
        animation.timeAccumulator -= animation.FRAME_DURATION;
        animation.frameIndex++;
        animation.frameIndex %= SpriteLoaderManager::GetInstance().GetSpriteSheet(enemyType).spriteFrameCount;
    }
}

void AEnemy::Update(float deltaTime)
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
