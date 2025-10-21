#include <cmath>
#include "WingWeapon.hpp"

// Helper lambdas para obtener datos del JSON


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

void WingWeapon::Attack(const Vector2& position, const Vector2& direction, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        // Disparo central
        ShootProjectile(position, direction);

        // Rotaciones ±45 grados
        constexpr float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
        const float angle = 45.0f * DEG_TO_RAD;
        const float cosA = std::cos(angle);
        const float sinA = std::sin(angle);

        Vector2 dirPlus{
            direction.x * cosA - direction.y * sinA,
            direction.x * sinA + direction.y * cosA
        };
        Vector2 dirMinus{
            direction.x * cosA + direction.y * sinA,
            -direction.x * sinA + direction.y * cosA
        };

        ShootProjectile(position, dirPlus);
        ShootProjectile(position, dirMinus);

        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> WingWeapon::CreateProjectile() {
    return std::make_unique<WingProjectile>();
}
