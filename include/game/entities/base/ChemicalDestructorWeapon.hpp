#pragma once
#include "ARangeWeapon.hpp"
#include <memory>

class ChemicalDestructorWeapon : public ARangeWeapon
{
public:
    /// @brief Constructor de la clase ChemicalDestructorWeapon que carga desde JSON
    ChemicalDestructorWeapon(const Vector2 &position, std::vector<AEnemy *> &enemiesInRange,
                             std::vector<AEnemy *> &allEnemies);
    virtual ~ChemicalDestructorWeapon();

    /// @brief Método para atacar
    void Attack(const Vector2 &position, float deltaTime) override;

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;
};
