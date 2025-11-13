#include "FollowPlayerSystem.hpp"
#include "FlockingComponent.hpp"
#include "FollowPlayerComponent.hpp"
#include "NormalTypeEnemiesComponents.hpp"
#include "TransformComponent.hpp"
#include "raymath.h"

void FollowEnemySystem::Update(entt::registry &registry, float deltaTime)
{
    auto view = registry.view<const ZombieComponent, const FollowPlayerComponent, const PositionComponent,
                              VelocityComponent, const MovementSpeedComponent, const FlockingComponent>();
    // De momento como el componente zombie no tiene nada dentro solo se usa como tag pero no se descompone
    for (auto [entity, targetPlayer, position, velocity, speed, flocking] : view.each())
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
            Vector2 targetForce = Vector2Scale(Vector2Normalize(toPlayer), speed.movementSpeed * flocking.targetWeight);
            velocity.velocity = Vector2Add(velocity.velocity, Vector2Scale(targetForce, deltaTime));
        }

        // Limitamos la velocidad maxima
        float currentSpeed = Vector2Length(velocity.velocity);
        if (currentSpeed > speed.movementSpeed)
        {
            velocity.velocity = Vector2Scale(Vector2Normalize(velocity.velocity), speed.movementSpeed);
        }
    }
}
