#pragma once
#include "ARangeWeapon.hpp"
#include "LaserRayProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class LaserRayWeapon : public ARangeWeapon {
private:
    /// @brief Offset del proyectil (mitad del ancho del sprite)
    float projectileOffset;

public:
    /// @brief Constructor de la clase LaserRayWeapon que carga desde JSON
    LaserRayWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~LaserRayWeapon();

    /// @brief Método para atacar
    void Attack(const Vector2& position, float deltaTime) override;

    void Attack() override {}

    void ShootProjectile(const Vector2 &position, const Vector2 &direction, const std::vector<AEnemy *> &allEnemies);

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};