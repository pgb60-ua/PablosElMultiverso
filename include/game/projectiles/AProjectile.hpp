#pragma once
#include "AEnemy.hpp"
#include "Stats.hpp"
#include "Geometry.hpp"
#include "SpriteAnimation.hpp"
extern "C"
{
#include <raylib.h>
}

class AProjectile
{
protected:
    Stats stats;
    Vector2 direction;
    bool active = false;
    SpriteAnimation animation;
    Shape shape;
    std::vector<AEnemy *> &enemiesInScene;

public:
    AProjectile(std::vector<AEnemy *> &allEnemies);
    virtual ~AProjectile() = default;

    /// @brief Actualiza la posición del proyectil
    /// @details Mueve el proyectil en la dirección especificada a una velocidad determinada por las estadísticas del proyectil.
    /// @note Para comprobar si se tiene que mover el proyectil, se debe comprobar si está activo.
    /// @param deltaTime
    virtual void update(float deltaTime);

    Vector2 getPosition() const;
    Vector2 getDirection() const { return direction; }
    Stats getStats() const { return stats; }

    /// @brief Funcion para renderizar el proyectil
    virtual void render() = 0;

    void deactivate();
    bool isActive() const { return active; }
    virtual void activate(Vector2 position, Vector2 direction, const Stats &stats);

    /// @brief Establece el radio de la hitbox circular del proyectil
    /// @param radius El radio en píxeles
    void setRadius(float radius);
};
