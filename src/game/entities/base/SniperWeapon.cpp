#include <cmath>
#include "SniperWeapon.hpp"
#include "WingProjectile.hpp"

// Helper lambdas para obtener datos del JSON


SniperWeapon::SniperWeapon(const Vector2& position, std::vector<AEnemy*>& enemiesInRange)
    : ARangeWeapon(
        GetStringFromJSON("name", WEAPON_TYPE::SNIPER, "Unknown Weapon"),
        GetStringFromJSON("description", WEAPON_TYPE::SNIPER, ""),
        DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::SNIPER),
        GetRarityFromJSON(WEAPON_TYPE::SNIPER),
        GetIntFromJSON("level", WEAPON_TYPE::SNIPER, 1),
        GetIntFromJSON("pool_size", WEAPON_TYPE::SNIPER, 50),
        position,
        enemiesInRange
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
        ShootProjectile(position, direction);
        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> SniperWeapon::CreateProjectile() {
    return std::make_unique<WingProjectile>();
}
