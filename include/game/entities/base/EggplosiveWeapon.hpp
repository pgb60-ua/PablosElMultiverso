#pragma once
#include "ARangeWeapon.hpp"
#include "EggplosiveProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class EggplosiveWeapon : public ARangeWeapon {
public:
    /// @brief Constructor de la clase EggplosiveWeapon que carga desde JSON
    EggplosiveWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~EggplosiveWeapon();
    
    /// @brief Método para atacar
    void Attack(const Vector2& position, float deltaTime) override;

    void Attack() override {}

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};