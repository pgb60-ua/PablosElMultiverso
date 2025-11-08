#pragma once
#include "ARangeWeapon.hpp"
#include "SniperProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class SniperWeapon : public ARangeWeapon {
public:
    /// @brief Constructor de la clase SniperWeapon que carga desde JSON
    SniperWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~SniperWeapon();  

    /// @brief Método para atacar
    void Attack(const Vector2& position, float deltaTime) override;

    void Attack() override {}

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};