#pragma once
#include "ARangeWeapon.hpp"
#include "WingProjectile.hpp"
#include "DataFileManager.hpp"
#include <memory>

class WingWeapon : public ARangeWeapon {
private:
    /// @brief tiempo desde el último ataque
    float timeSinceLastAttack = 0.0f;
public:
    /// @brief Constructor de la clase WingWeapon que carga desde JSON
    WingWeapon(const Vector2& position = {0.0f, 0.0f});
    virtual ~WingWeapon();  

    /// @brief Método para atacar
    void Attack(const Vector2& position, const Vector2& direction, float deltaTime) override;

    void Attack() override {}

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};