#include "AccelerationIntegrationSystem.hpp"
#include "AccelerationComponent.hpp"
#include "FlockingComponent.hpp"
#include "TransformComponent.hpp"
#include <raymath.h>

void AccelerationIntegrationSystem::Update(entt::registry &registry, float deltaTime)
{
    auto view =
        registry
            .view<AccelerationComponent, VelocityComponent, const MovementSpeedComponent, const FlockingComponent>();

    for (auto [entity, acceleration, velocity, speed, flocking] : view.each())
    {
        // Limitar la fuerza máxima de aceleración
        float maxForce = speed.movementSpeed * flocking.maxForceMultiplier;
        if (Vector2Length(acceleration) > maxForce)
        {
            acceleration = Vector2Scale(Vector2Normalize(acceleration), maxForce);
        }

        // Aplicar aceleración a velocidad
        velocity.velocity = Vector2Add(velocity.velocity, Vector2Scale(acceleration, deltaTime));

        // Resetear aceleración para el próximo frame
        acceleration = Vector2Zero();
    }
}
