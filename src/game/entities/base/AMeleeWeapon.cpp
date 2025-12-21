#include "AMeleeWeapon.hpp"
#include <algorithm>
#include <cmath>

AMeleeWeapon::AMeleeWeapon(const std::string &name, const std::string &description,
                           const Stats &stats, ItemRarity itemRarity, int level,
                           const Vector2 &position, std::vector<AEnemy *> &enemiesInRange,
                           std::vector<AEnemy *> &allEnemies)
    : AWeapon(name, description, stats, itemRarity, level, position, enemiesInRange, allEnemies)
{
    UpdateAttackInterval();
}

AMeleeWeapon::~AMeleeWeapon() {}

void AMeleeWeapon::Attack(float deltaTime)
{
    timeSinceLastAttack += deltaTime;
    
    if (timeSinceLastAttack >= attackInterval && !enemiesInRange.empty())
    {
        // Ataca a todos los enemigos en rango
        for (auto &enemy : enemiesInRange)
        {
            if (enemy != nullptr)
            {
                enemy->TakeDamage(stats);
            }
        }
        timeSinceLastAttack -= attackInterval;
    }
}

void AMeleeWeapon::UpdateAttackInterval()
{
    float attackSpeed = stats.GetOffensiveStats().attackSpeed;
    attackSpeed = std::max(MIN_ATTACK_SPEED, attackSpeed);
    attackInterval = 1.0f / attackSpeed;
}

void AMeleeWeapon::setStats(const Stats &newStats)
{
    stats = newStats;
    UpdateAttackInterval();
}

void AMeleeWeapon::render()
{
    AWeapon::render();
}

void AMeleeWeapon::update(float deltaTime, const Vector2 &position)
{
    AWeapon::update(deltaTime, position);
    Attack(deltaTime);
}
