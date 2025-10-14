#include <RangeWeapon.hpp>

RangeWeapon::RangeWeapon(const std::string& name, const std::string& description, 
    const Stats& stats, ItemRarity itemRarity, int level, size_t poolSize)
    : Weapon(name, description, stats, itemRarity, WeaponType::Ranged, level), POOL_SIZE(poolSize) {
    InitializeProjectilePool();
}

RangeWeapon::~RangeWeapon() {
}

// AQUI CONSIDERO QUE ACTIVATE DEBERIA LLEVAR TAMBIÉN UNA DIRECCIÓN HACIA DONDE SE DISPARA, PERO MANUEL NO LO HIZO ASÍ, EN LA PR QUE SE REVISE
void RangeWeapon::ShootProjectile(const Vector2& position, const Vector2& direction) {
    AProjectile* projectile = GetProjectileFromPool();
    if (projectile) {
        projectile->activate(position, stats);
    }
}

void RangeWeapon::UpdateProjectiles(float deltaTime) {
    for (const auto& projectile : projectilePool) {
        if (projectile->isActive()) {
            projectile->update(deltaTime);
        }
    }
}

std::vector<AProjectile*> RangeWeapon::GetActiveProjectiles() const {
    std::vector<AProjectile*> activeProjectiles;
    for (const auto& projectile : projectilePool) {
        if (projectile->isActive()) {
            activeProjectiles.push_back(projectile.get());
        }
    }
    return activeProjectiles;
}

void RangeWeapon::InitializeProjectilePool() {
    projectilePool.reserve(POOL_SIZE);
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        projectilePool.push_back(CreateProjectile());
    }
}

AProjectile* RangeWeapon::GetProjectileFromPool() {
    for (size_t i = 0; i < POOL_SIZE; ++i) {
        size_t index = (currentProjectileIndex + i) % POOL_SIZE;
        if (!projectilePool[index]->isActive()) {
            currentProjectileIndex = (index + 1) % POOL_SIZE;
            return projectilePool[index].get();
        }
    }
    return nullptr; // No hay proyectiles disponibles
}