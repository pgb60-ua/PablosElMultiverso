#include <cmath>
#include "WingWeapon.hpp"

WingWeapon::WingWeapon(const Vector2& position)
    : ARangeWeapon(
        GetStringFromJSON("name", WEAPON_TYPE::WING, "Unknown Weapon"),
        GetStringFromJSON("description", WEAPON_TYPE::WING, ""),
        DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::WING),
        GetRarityFromJSON(WEAPON_TYPE::WING),
        GetIntFromJSON("level", WEAPON_TYPE::WING, 1),
        GetIntFromJSON("pool_size", WEAPON_TYPE::WING, 50)
    )
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::WING);
}

WingWeapon::~WingWeapon() {
}

void WingWeapon::Attack(const Vector2& position, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        // Disparo central
        ShootProjectile(position, direction);

        
        const float angle = 30.0f * DEG2RAD;
        const float cosA = std::cos(angle);
        const float sinA = std::sin(angle);

        Vector2 dirPlus{
            direction.x * cosA - direction.y * sinA,
            direction.x * sinA + direction.y * cosA
        };
        Vector2 dirMinus{
            direction.x * cosA + direction.y * sinA,
            direction.x * -sinA + direction.y * cosA
        };

        ShootProjectile(position, dirPlus);
        ShootProjectile(position, dirMinus);

        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> WingWeapon::CreateProjectile() {
    return std::make_unique<WingProjectile>();
}
