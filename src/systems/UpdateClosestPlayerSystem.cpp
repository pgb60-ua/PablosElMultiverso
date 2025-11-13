#include "UpdateClosestPlayerSystem.hpp"
#include "FollowPlayerComponent.hpp"
#include "PlayerComponent.hpp"
#include "TransformComponent.hpp"
#include "spdlog/spdlog.h"
#include <limits>

void UpdateClosestPlayerSystem::Update(entt::registry &registry, float deltaTime)
{
    timeSinceLastUpdate += deltaTime;

    if (timeSinceLastUpdate < UPDATE_INTERVAL)
        return;

    // Reiniciamos el timer
    timeSinceLastUpdate -= UPDATE_INTERVAL;

    auto players = registry.view<const PlayerComponent, const PositionComponent>();

    auto enemies = registry.view<FollowPlayerComponent, const PositionComponent>();

    for (auto [entity, targetPlayer, enemyPosition] : enemies.each())
    {
        entt::entity closestPlayer = entt::null;
        // Lo iniciamos a max para luego ir reduciendolo
        float minDistance = std::numeric_limits<float>::max();

        // Buscamos el jugador mas cercano
        for (auto [entityPlayer, player, playerPosition] : players.each())
        {
            float dx = playerPosition.x - enemyPosition.x;
            float dy = playerPosition.y - enemyPosition.y;
            float distanceSq = dx * dx + dy * dy; // Distancia al cuadrado (para mas rapido)

            // Vamos comparando que player esta mas cerca
            if (distanceSq < minDistance)
            {
                minDistance = distanceSq;
                closestPlayer = entityPlayer;
            }
        }
        // Actualizamos el target en el componente
        targetPlayer.targetPlayer = closestPlayer;
    }
}
