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
    /// @param objectives Referencia al vector de jugadores (objetivos)
    Zombie(std::vector<Player *> objectives);
};
