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
        enemiesInRange.erase(
            std::remove(enemiesInRange.begin(), enemiesInRange.end(), nullptr),
            enemiesInRange.end());
        // Ataca a todos los enemigos en rango
        for (AEnemy *enemy : enemiesInRange)
        {
            enemy->TakeDamage(stats);
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

void AMeleeWeapon::SetStats(const Stats &newStats)
{
    stats = newStats;
    UpdateAttackInterval();
}

void AMeleeWeapon::update(float deltaTime, const Vector2 &position)
{
    AWeapon::update(deltaTime, position);

    //Las armas melee apuntan hacia fuera, no como las armas de distancia
    float rad = currentOrbitAngle * DEG2RAD;
    Vector2 outwardDir = { cosf(rad), sinf(rad) };
    SetDirection(outwardDir);

    Attack(deltaTime);
}