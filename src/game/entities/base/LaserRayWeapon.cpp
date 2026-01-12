#include <cmath>
#include "LaserRayWeapon.hpp"
#include "SpriteLoaderManager.hpp"
#include "AudioManager.hpp"

LaserRayWeapon::LaserRayWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies)
    : ARangeWeapon(
        GetStringFromJSON("name", WEAPON_TYPE::LASER_RAY, "Unknown Weapon"),
        GetStringFromJSON("description", WEAPON_TYPE::LASER_RAY, ""),
        DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::LASER_RAY),
        GetRarityFromJSON(WEAPON_TYPE::LASER_RAY),
        GetIntFromJSON("level", WEAPON_TYPE::LASER_RAY, 1),
        GetIntFromJSON("pool_size", WEAPON_TYPE::LASER_RAY, 4),
        position,
        enemiesInRange,
        allEnemies
    ), projectileOffset(0.0f)
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::LASER_RAY);
    
    const SpriteSheet &projectileSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
    if (!projectileSheet.frames.empty())
    {
        projectileOffset = projectileSheet.frames[0].width / 2.0f;
    }
}

LaserRayWeapon::~LaserRayWeapon() {
}

void LaserRayWeapon::Attack(const Vector2& position, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        ShootProjectile(position, direction, allEnemies);
        AudioManager::GetInstance().PlaySound(WEAPON_TYPE::LASER_RAY);
        timeSinceLastAttack -= attackInterval;
    }
    
    for (auto &projectile : GetActiveProjectiles())
    {
        Vector2 newLaserPos = {
            position.x + direction.x * projectileOffset,
            position.y + direction.y * projectileOffset
        };
        
        LaserRayProjectile* laser = static_cast<LaserRayProjectile*>(projectile);
        laser->updatePositionAndDirection(newLaserPos, direction);
    }
}

void LaserRayWeapon::ShootProjectile(const Vector2 &position, const Vector2 &direction, const std::vector<AEnemy *> &allEnemies)
{
    AProjectile *projectile = GetProjectileFromPool();
    if (projectile)
    {
        projectile->activate(position, direction, stats);
    }
}

std::unique_ptr<AProjectile> LaserRayWeapon::CreateProjectile() {
    return std::make_unique<LaserRayProjectile>(allEnemies);
}
