#pragma once

#include "AEnemy.hpp"
extern "C"
{
#include <raylib.h>
}

/// @brief Clase que representa un enemigo de tipo Zombie
/// El zombie es un enemigo básico que se mueve lentamente hacia el jugador
/// y realiza ataques cuerpo a cuerpo
class Zombie : public AEnemy
{
public:
    /// @brief Constructor del Zombie
    /// @param hitbox Hitbox del zombie
    /// @param objectives Referencia al vector de jugadores (objetivos)
    /// @param pabloCoinsAtDeath Cantidad de Pablo Coins que suelta al morir
    /// Las estadísticas se cargan automáticamente desde el archivo zombie.json
    Zombie(std::vector<Player *> objectives);

    /*--------------------------*/
    // Métodos Heredados de AEntity
    /*--------------------------*/

    /// @brief Actualiza el estado del zombie
    /// Se llama en cada frame para aplicar la IA y el movimiento
    void Update(float deltaTime) override;

    /*--------------------------*/
    // Métodos Heredados de AEnemy
    /*--------------------------*/

    /// @brief Renderiza el zombie
    void Render() override;
};
