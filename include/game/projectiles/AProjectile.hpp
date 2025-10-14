#pragma once
#include "Stats.hpp"
extern "C"
{
#include <raylib.h>
}

class AProjectile
{
private:
    Stats stats;
    Vector2 position;
    Vector2 direction;

public:
    AProjectile() = default;
    ~AProjectile() = default;

    /// @brief Actualiza la posición del proyectil
    /// @details Mueve el proyectil en la dirección especificada a una velocidad determinada por las estadísticas del proyectil.
    /// @note Para comprobar si se tiene que mover el proyectil, se debe comprobar si está activo(tiene una posicion destino válida).
    /// @param deltaTime
    virtual void update(float deltaTime) = 0;

    Vector2 getPosition() const;
    Stats getStats() const;

    void deactivate();
    void activate(Vector2 position, Vector2 direction, const Stats &stats);
};