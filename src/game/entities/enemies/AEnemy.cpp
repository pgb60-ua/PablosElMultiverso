#include "AEnemy.hpp"

AEnemy::AEnemy(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath)
    : AEntity(stats, hitbox), objectives(objectives), pabloCoinsAtDeath(pabloCoinsAtDeath)
{
}

void AEnemy::SetObjective(const std::vector<Player *> newObjectives) { objectives = newObjectives; }

void AEnemy::SetPabloCoinsAtDeath(int cantidad) { pabloCoinsAtDeath = cantidad; }

int AEnemy::GetPabloCoinsAtDeath() const { return pabloCoinsAtDeath; }

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
