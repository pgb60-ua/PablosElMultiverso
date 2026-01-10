#pragma once

#include "Geometry.hpp"
#include "SpriteAnimation.hpp"
#include "Stats.hpp"
extern "C"
{
#include <raylib.h>
}

// Clase que representa lo mas abstracto de un personaje y enemigo que tienen
// común
class AEntity
{
private:
    static constexpr float MIN_HEALTH_BASE = 1.0f;

protected:
    /// @brief Variable para almacenar todas las stats de una entidad
    Stats stats;

    /// @brief Hitbox de la entidad (rectangulo - circulo - triangulo)
    Shape hitbox;

    /// @brief Propiedad autocalculada, tiempo que ha de pasar para volver atacar
    float attackCooldown;

    /// @brief Variable que almacena la suma de deltaTime para comparar si ha de atacar
    float currentAttackCooldownTime;

    float receiveDamageCooldownTime = 0.0f;

    SpriteAnimation animation;

    AEntity(Stats stats, const Shape &hitbox);

public:
    /// @brief Establece la estadistica de attackSpeed con logica previa
    void SetAttackSpeed(float newAttackSpeed);

    /// @brief Obtiene el tiempo de recarga del ataque actual
    float GetAttackCooldown() const { return attackCooldown; }

    Vector2 GetPosition() const
    {
        switch (hitbox.type)
        {
        case SHAPE_RECTANGLE:
            return Vector2{hitbox.data.rectangle.x, hitbox.data.rectangle.y};
        case SHAPE_CIRCLE:
            return hitbox.data.circle.center;
        default:
            return Vector2{0, 0};
        }
    }

    void SetPosition(Vector2 newPosition)
    {
        switch (hitbox.type)
        {
        case SHAPE_RECTANGLE:
            hitbox.data.rectangle.x = newPosition.x;
            hitbox.data.rectangle.y = newPosition.y;
            break;
        case SHAPE_CIRCLE:
            hitbox.data.circle.center = newPosition;
            break;
        default:
            break;
        }
    }

    /*--------------------------*/
    // Hitbox
    /*--------------------------*/

    /// @brief Devuelve la hitbox de la entidad
    Shape GetHitbox() const { return hitbox; };

    /// @brief Establece la hitbox de la entidad a un rectangulo
    void SetRectangleHitbox(Rectangle rectangle)
    {
        hitbox.type = SHAPE_RECTANGLE;
        hitbox.data.rectangle = rectangle;
    };

    /// @brief Establece la hitbox de la entidad a un circulo
    void SetCircleHitbox(Vector2 center, float radius)
    {
        hitbox.type = SHAPE_CIRCLE;
        hitbox.data.circle = {center, radius};
    };
    void TakeDamage(const Stats &stats);

    virtual void Render() = 0;
    /// @brief Devuelve si la entidad tiene mas de 0 de vida
    bool IsAlive();

    /// @brief Realiza un ataque al jugador
    virtual bool Attack();
    /// @brief Comprueba las colisiones de la entidad
    virtual void CheckCollisions(float deltaTime) = 0;

    virtual void Update(float deltaTime) = 0;
    virtual ~AEntity() {};
};
