#include "WingWeapon.hpp"

WingWeapon::WingWeapon(const std::string& name, const std::string& description, 
    const Stats& stats, ItemRarity itemRarity, int level, float fireRate, size_t poolSize)
    : ARangeWeapon(name, description, stats, itemRarity, level, poolSize) {}

WingWeapon::~WingWeapon() {
}

void WingWeapon::Attack(const Vector2& position, const Vector2& direction, float deltaTime) {
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval) {
        ShootProjectile(position, direction);
        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> WingWeapon::CreateProjectile() {
    return std::make_unique<WingProjectile>();
}
