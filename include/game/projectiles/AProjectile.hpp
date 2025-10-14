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
    bool active = false;
    float speed;

public:
    AProjectile();
    virtual ~AProjectile();

    /// @brief Actualiza la posición del proyectil
    /// @details Mueve el proyectil en la dirección especificada a una velocidad determinada por las estadísticas del proyectil.
    /// @note Para comprobar si se tiene que mover el proyectil, se debe comprobar si está activo.
    /// @param deltaTime
    virtual void update(float deltaTime);

    Vector2 getPosition() const { return position; }
    Vector2 getDirection() const { return direction; }
    Stats getStats() const { return stats; }

    /// @brief Funcion para renderizar el proyectil
    virtual void render() const = 0;

    void deactivate();
    bool isActive() const { return active; }
    void activate(Vector2 position, Vector2 direction, const Stats &stats);
};