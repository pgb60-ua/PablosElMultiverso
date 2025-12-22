#include <cmath>
#include "SniperWeapon.hpp"
#include "AudioManager.hpp"

// Helper lambdas para obtener datos del JSON


SniperWeapon::SniperWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies)
    : ARangeWeapon(
        GetStringFromJSON("name", WEAPON_TYPE::SNIPER, "Unknown Weapon"),
        GetStringFromJSON("description", WEAPON_TYPE::SNIPER, ""),
        DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::SNIPER),
        GetRarityFromJSON(WEAPON_TYPE::SNIPER),
        GetIntFromJSON("level", WEAPON_TYPE::SNIPER, 1),
        GetIntFromJSON("pool_size", WEAPON_TYPE::SNIPER, 50),
        position,
        enemiesInRange,
        allEnemies
    )
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::SNIPER);
}

SniperWeapon::~SniperWeapon() {
}

void SniperWeapon::Attack(const Vector2& position, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        ShootProjectile(position, direction, allEnemies);
        AudioManager::GetInstance().PlaySound(PROJECTILE_TYPE::SNIPER);
        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> SniperWeapon::CreateProjectile() {
    return std::make_unique<SniperProjectile>(allEnemies);
}
