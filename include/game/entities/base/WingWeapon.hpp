#pragma once
#include "ARangeWeapon.hpp"
#include "WingProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class WingWeapon : public ARangeWeapon {
public:
    /// @brief Constructor de la clase WingWeapon que carga desde JSON
    WingWeapon(const Vector2 &position, std::vector<AEnemy *> &enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~WingWeapon();

    /// @brief Método para atacar
    void Attack(const Vector2 &position, float deltaTime) override;

    void Attack() override {}

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};