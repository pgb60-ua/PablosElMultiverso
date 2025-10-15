#pragma once
#include "ARangeWeapon.hpp"
#include "WingProjectile.hpp"

class WingWeapon : public ARangeWeapon {
private:
    /// @brief tiempo desde el ultimo ataque
    float timeSinceLastAttack;

    /// @brief Cadencia de disparo (ataques por segundo)
    const float FIRE_RATE; // ataques por segundo

public:
    /// @brief Constructor de la clase WingWeapon
    WingWeapon(const std::string& name, const std::string& description, 
        const Stats& stats, ItemRarity itemRarity, int level, float fireRate = 1.5f, size_t poolSize = 50);
    virtual ~WingWeapon();  

    /// @brief Método para atacar
    void Attack(const Vector2& position, const Vector2& direction);

    /// @brief Método para crear proyectiles
    virtual std::unique_ptr<AProjectile> CreateProjectile() override;
};