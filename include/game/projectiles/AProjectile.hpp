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
private:
    Stats stats;
    Shape shape;
    Vector2 direction;
    bool active = false;
    float speed;
    const std::vector<AEnemy *> *enemiesInScene;

protected:
    SpriteAnimation animation;

public:
    AProjectile();
    virtual ~AProjectile();

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
    void activate(Vector2 position, Vector2 direction, const Stats &stats, const std::vector<AEnemy *> &allEnemies);
};