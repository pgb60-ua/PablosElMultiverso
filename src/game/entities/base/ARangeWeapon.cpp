#include "ARangeWeapon.hpp"

ARangeWeapon::ARangeWeapon(const std::string &name, const std::string &description,
                           const Stats &stats, ItemRarity itemRarity, int level, size_t poolSize, const Vector2 &position,
                           const std::vector<AEnemy *> &enemiesInRange, const std::vector<AEnemy *> &allEnemies)
    : AWeapon(name, description, stats, itemRarity, level, position, enemiesInRange), POOL_SIZE(poolSize)
{
    UpdateAttackInterval();
}

ARangeWeapon::~ARangeWeapon()
{
}

// AQUI CONSIDERO QUE ACTIVATE DEBERIA LLEVAR TAMBIÉN UNA DIRECCIÓN HACIA DONDE SE DISPARA, PERO MANUEL NO LO HIZO ASÍ, EN LA PR QUE SE REVISE
// Le dijo la sarten al cazo
void ARangeWeapon::ShootProjectile(const Vector2 &position, const Vector2 &direction, const std::vector<AEnemy *> &allEnemies)
{
    AProjectile *projectile = GetProjectileFromPool();
    if (projectile)
    {
        projectile->activate(position, direction, stats, allEnemies);
    }
}

void ARangeWeapon::UpdateProjectiles(float deltaTime)
{
    for (const auto &projectile : projectilePool)
    {
        if (projectile->isActive())
        {
            projectile->update(deltaTime);
        }
    }
}

std::vector<AProjectile *> ARangeWeapon::GetActiveProjectiles() const
{
    std::vector<AProjectile *> activeProjectiles;
    for (const auto &projectile : projectilePool)
    {
        if (projectile->isActive())
        {
            activeProjectiles.push_back(projectile.get());
        }
    }
    return activeProjectiles;
}

void ARangeWeapon::InitializeProjectilePool()
{
    projectilePool.reserve(POOL_SIZE);
    for (size_t i = 0; i < POOL_SIZE; ++i)
    {
        projectilePool.push_back(CreateProjectile());
    }
}

AProjectile *ARangeWeapon::GetProjectileFromPool()
{
    for (size_t i = 0; i < POOL_SIZE; ++i)
    {
        size_t index = (currentProjectileIndex + i) % POOL_SIZE;
        if (!projectilePool[index]->isActive())
        {
            currentProjectileIndex = (index + 1) % POOL_SIZE;
            return projectilePool[index].get();
        }
    }
    return nullptr; // No hay proyectiles disponibles
}

void ARangeWeapon::SetStats(const Stats &newStats)
{
    stats = newStats;
    UpdateAttackInterval();
}

void ARangeWeapon::UpdateAttackInterval()
{
    float attackSpeed = stats.GetOffensiveStats().attackSpeed;
    attackSpeed = std::max(MIN_ATTACK_SPEED, attackSpeed);
    attackInterval = 1.0f / attackSpeed;
}

void ARangeWeapon::render()
{
    AWeapon::render();
    for (const auto &projectile : GetActiveProjectiles())
    {
        projectile->render();
    }
}

void ARangeWeapon::update(float deltaTime, const Vector2 &position)
{
    AWeapon::update(deltaTime, position);
    Attack(position, deltaTime); // TODO : La dirección debería de ser calculada
    UpdateProjectiles(deltaTime);
}