#include "WingWeapon.hpp"

WingWeapon::WingWeapon(const std::string& name, const std::string& description, 
    const Stats& stats, ItemRarity itemRarity, int level, float fireRate, size_t poolSize)
    : ARangeWeapon(name, description, stats, itemRarity, level, poolSize), 
    FIRE_RATE(fireRate) {
}

WingWeapon::~WingWeapon() {
}

void WingWeapon::Attack(const Vector2& position, const Vector2& direction) {
    timeSinceLastAttack += GetFrameTime();
    float attackInterval = FIRE_RATE / std::max(0.1f,stats.GetAttackSpeed());

    if (timeSinceLastAttack >= attackInterval) {
        ShootProjectile(position, direction);
        timeSinceLastAttack = 0.0f;
    }
}

std::unique_ptr<AProjectile> WingWeapon::CreateProjectile() {
    return std::make_unique<WingProjectile>();
}
