#include "FollowPlayerSystem.hpp"
#include "AccelerationComponent.hpp"
#include "FlockingComponent.hpp"
#include "FollowPlayerComponent.hpp"
#include "NormalTypeEnemiesComponents.hpp"
#include "TransformComponent.hpp"
#include "raymath.h"

void FollowEnemySystem::Update(entt::registry &registry)
{
    auto view = registry.view<const ZombieComponent, const FollowPlayerComponent, const PositionComponent,
                              AccelerationComponent, const MovementSpeedComponent, const FlockingComponent>();
    // De momento como el componente zombie no tiene nada dentro solo se usa como tag pero no se descompone
    for (auto [entity, targetPlayer, position, acceleration, speed, flocking] : view.each())
    {
        // Con esto compruebo que la "id" que guardo del target existe en el registro
        if (!registry.valid(targetPlayer.targetPlayer))
            continue;
        // Intento obtener la posicion del player que tiene como target
        auto *playerPos = registry.try_get<PositionComponent>(targetPlayer.targetPlayer);
        if (!playerPos)
            continue;

        // Calculamos direccion hacia el jugador
        Vector2 zombiePos = {position.x, position.y};
        Vector2 targetPos = {playerPos->x, playerPos->y};
        Vector2 toPlayer = Vector2Subtract(targetPos, zombiePos);

        if (Vector2Length(toPlayer) > 0.0f)
        {
            // SUMAR a la aceleración (no a velocity directamente)
            Vector2 targetForce = Vector2Scale(Vector2Normalize(toPlayer), speed.movementSpeed * flocking.targetWeight);
            acceleration = Vector2Add(acceleration, targetForce);
        }
    }
}
