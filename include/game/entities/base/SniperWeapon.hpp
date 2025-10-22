#pragma once
#include "ARangeWeapon.hpp"
#include "SniperProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class SniperWeapon : public ARangeWeapon {
private:
    /// @brief tiempo desde el último ataque
    float timeSinceLastAttack = 0.0f;
public:
    /// @brief Constructor de la clase SniperWeapon que carga desde JSON
    SniperWeapon(const Vector2& position = {0.0f, 0.0f});
    virtual ~SniperWeapon();  

    /// @brief Método para atacar
    void Attack(const Vector2& position, const Vector2& direction, float deltaTime) override;

    void Attack() override {}

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};