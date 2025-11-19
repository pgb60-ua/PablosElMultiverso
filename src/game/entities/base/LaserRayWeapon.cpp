#include <cmath>
#include "LaserRayWeapon.hpp"
#include "SpriteLoaderManager.hpp"

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
    )
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::LASER_RAY);
}

LaserRayWeapon::~LaserRayWeapon() {
}

void LaserRayWeapon::Attack(const Vector2& position, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        ShootProjectile(position, direction, allEnemies);
        timeSinceLastAttack -= attackInterval;
    }
}

void LaserRayWeapon::ShootProjectile(const Vector2 &position, const Vector2 &direction, const std::vector<AEnemy *> &allEnemies)
{
    AProjectile *projectile = GetProjectileFromPool();
    if (projectile)
    {
        const SpriteSheet &projectileSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::LASER_RAY);
        
        float projectileOffset = 0.0f;
        if (!projectileSheet.frames.empty())
        {
            projectileOffset = projectileSheet.frames[0].width / 2.0f;
        }
        
        // Calcular la posición inicial del proyectil
        Vector2 projectileStartPos = {
            position.x + direction.x * projectileOffset,
            position.y + direction.y * projectileOffset
        };
        
        projectile->activate(projectileStartPos, direction, stats);
    }
}

std::unique_ptr<AProjectile> LaserRayWeapon::CreateProjectile() {
    return std::make_unique<LaserRayProjectile>(allEnemies);
}
