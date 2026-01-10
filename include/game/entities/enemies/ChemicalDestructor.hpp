#pragma once

#include "AEnemy.hpp"
#include "ChemicalDestructorWeapon.hpp"
#include <vector>

class ChemicalDestructor : public AEnemy
{
private:
    /// @brief Arma del ChemicalDestructor
    std::unique_ptr<ChemicalDestructorWeapon> weapon;
    
    /// @brief Distancia objetivo que el enemigo intenta mantener del jugador
    static constexpr float TARGET_DISTANCE = 500.0f; // Ajusta este valor según necesites

    /// @brief Obtiene la posición del centro del hitbox del enemigo
    /// @return Vector2 con las coordenadas del centro del hitbox
    Vector2 GetCenterPosition() const;

public:
    /// @brief Constructor del ChemicalDestructor
    /// @param players Referencia al vector de jugadores (objetivos)
    ChemicalDestructor(std::vector<Player *> players);

    /// @brief Método de ataque del ChemicalDestructor
    bool Attack() override;

    /// @brief Actualiza el estado del ChemicalDestructor
    void Update(float deltaTime) override;
    
    /// @brief Calcula la fuerza hacia el jugador manteniendo una distancia objetivo
    Vector2 CalculateTargetForce(const Vector2 &enemyPos, const Vector2 &playerPos, float baseSpeed) override;

    /// @brief Renderiza el ChemicalDestructor
    void Render() override;
};
