#pragma once
#include "ARangeWeapon.hpp"
#include "Player.hpp"
#include <memory>

class ChemicalDestructorWeapon : public ARangeWeapon
{
private:
    /// @brief Referencia a los jugadores en el juego
    std::vector<Player *> &players;

public:
    /// @brief Constructor de la clase ChemicalDestructorWeapon que carga desde JSON
    ChemicalDestructorWeapon(const Vector2 &position, std::vector<AEnemy *> &enemies, std::vector<Player *> &players);
    virtual ~ChemicalDestructorWeapon();

    /// @brief Método para atacar
    void Attack(const Vector2 &position, float deltaTime) override;

    /// @brief Se 'implementa' pq es virtual puro
    void Attack() override;

    /// @brief Calcula la dirección de ataque normalizada
    Vector2 CalculateDirection() override;

    /// @brief Método para crear proyectiles
    std::unique_ptr<AProjectile> CreateProjectile() override;

    // /// @brief Renderiza solo los proyectiles, no el sprite del arma
    // void render() override;
};
