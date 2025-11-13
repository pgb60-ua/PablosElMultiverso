#include "FlockingSystem.hpp"
#include "AccelerationComponent.hpp"
#include "FlockingComponent.hpp"
#include "NormalTypeEnemiesComponents.hpp"
#include "TransformComponent.hpp"
#include "raymath.h"

void FlockingSystem::Update(entt::registry &registry)
{
    auto zombies = registry.view<const ZombieComponent, const PositionComponent, const VelocityComponent,
                                 AccelerationComponent, const FlockingComponent, const MovementSpeedComponent>();
    for (auto [entity, position, velocity, acceleration, flocking, speed] : zombies.each())
    {
        Vector2 separationForce = Vector2Zero();
        Vector2 alignmentForce = Vector2Zero();
        Vector2 cohesionForce = Vector2Zero();
        int neighborCount = 0;

        Vector2 currentPos = {position.x, position.y};

        // Resetear aceleración al inicio (se acumula en este frame)
        acceleration = Vector2Zero();

        for (auto [otherEntity, otherPosition, otherVelocity, otherAcceleration, otherFlocking, otherSpeed] :
             zombies.each())
        {
            if (entity == otherEntity)
                continue;

            Vector2 otherCurrentPosition = {otherPosition.x, otherPosition.y};
            Vector2 offset = Vector2Subtract(otherCurrentPosition, currentPos);
            float distance = Vector2Length(offset);

            // Si esta fuera del radio de percepcion ignorar
            if (distance <= 0.f || distance > flocking.perceptionRadius)
                continue;

            neighborCount++;

            // COHESION : acumulamos posiciones
            cohesionForce = Vector2Add(cohesionForce, otherCurrentPosition);

            // ALIGNMENT : acumulamos velocidades
            alignmentForce = Vector2Add(alignmentForce, otherVelocity.velocity);

            // SEPARATION : si esta muy cerca, se aleja
            if (distance < flocking.separationRadius)
            {
                Vector2 away = Vector2Scale(Vector2Normalize(offset), -1.0f);
                float falloff = (flocking.separationRadius - distance) / flocking.separationRadius;
                separationForce = Vector2Add(separationForce, Vector2Scale(away, falloff));
            }
        }

        // Aplicamos cohesion si hay vecinos
        if (neighborCount > 0)
        {
            // COHESION : moverse hacia el centro del grupo
            cohesionForce = Vector2Scale(cohesionForce, 1.0f / neighborCount);
            cohesionForce = Vector2Subtract(cohesionForce, currentPos);
            if (Vector2Length(cohesionForce) > 0.0f)
            {
                cohesionForce =
                    Vector2Scale(Vector2Normalize(cohesionForce), flocking.cohesionWeight * speed.movementSpeed);
                acceleration = Vector2Add(acceleration, cohesionForce);
            }

            // ALINEACION : moverse en la misma direccion que vecinos
            alignmentForce = Vector2Scale(alignmentForce, 1.0f / neighborCount);
            if (Vector2Length(alignmentForce) > 0.0f)
            {
                alignmentForce =
                    Vector2Scale(Vector2Normalize(alignmentForce), flocking.alignmentWeight * speed.movementSpeed);
                acceleration = Vector2Add(acceleration, alignmentForce);
            }
        }

        // Aplicamos separacion
        if (Vector2Length(separationForce) > 0.0f)
        {
            separationForce =
                Vector2Scale(Vector2Normalize(separationForce), flocking.separationWeight * speed.movementSpeed);
            acceleration = Vector2Add(acceleration, separationForce);
        }
    }
}
