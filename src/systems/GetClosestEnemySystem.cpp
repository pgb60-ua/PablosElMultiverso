#include "GetClosestEnemySystem.hpp"
#include "EnemyComponent.hpp"
#include "EntityComponent.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"
#include <limits>

void GetClosestEnemySystem::Update(entt::registry &registry, float deltaTime)
{
    timeSinceLastUpdate += deltaTime;

    if (timeSinceLastUpdate < UPDATE_INTERVAL)
    {
        return;
    }

    timeSinceLastUpdate -= UPDATE_INTERVAL;

    // Obtener todas las armas
    auto weapons = registry.view<WeaponComponent, const PositionComponent>();

    // Obtener todos los enemigos vivos
    auto enemies = registry.view<const EnemyComponent, const PositionComponent, const EntityComponent>();

    for (auto [weaponEntity, weapon, weaponPos] : weapons.each())
    {
        entt::entity closestEnemy = entt::null;
        float minDistanceSq = std::numeric_limits<float>::max();

        // Buscar el enemigo más cercano (vivo)
        for (auto [enemyEntity, enemy, enemyPos, entityState] : enemies.each())
        {
            // Solo considerar enemigos vivos
            if (entityState.state != ALIVE)
            {
                continue;
            }

            float dx = enemyPos.x - weaponPos.x;
            float dy = enemyPos.y - weaponPos.y;
            float distanceSq = dx * dx + dy * dy;

            if (distanceSq < minDistanceSq)
            {
                minDistanceSq = distanceSq;
                closestEnemy = enemyEntity;
            }
        }

        // Actualizar el target del arma
        weapon.targetEnemy = closestEnemy;
    }
}
