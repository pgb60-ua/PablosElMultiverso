#include "EggplosiveWeapon.hpp"
#include "AudioManager.hpp"

EggplosiveWeapon::EggplosiveWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies)
    : ARangeWeapon(
        GetStringFromJSON("name", WEAPON_TYPE::EGGPLOSIVE, ""),
        GetStringFromJSON("description", WEAPON_TYPE::EGGPLOSIVE, "Unknown Weapon"),
        DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::EGGPLOSIVE),
        GetRarityFromJSON(WEAPON_TYPE::EGGPLOSIVE),
        GetIntFromJSON("level", WEAPON_TYPE::EGGPLOSIVE, 1),
        GetIntFromJSON("pool_size", WEAPON_TYPE::EGGPLOSIVE, 10),
        position,
        enemiesInRange,
        allEnemies
    )
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::EGGPLOSIVE);
}

EggplosiveWeapon::~EggplosiveWeapon() 
{
}

void EggplosiveWeapon::Attack(const Vector2& position, float deltaTime) 
{
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) 
    {
        ShootProjectile(position, direction, allEnemies);
        AudioManager::GetInstance().PlaySound(PROJECTILE_TYPE::EGGPLOSIVE_BULLET);
        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> EggplosiveWeapon::CreateProjectile() 
{
    return std::make_unique<EggplosiveProjectile>(allEnemies);
}
