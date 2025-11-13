#include "WeaponRotationSystem.hpp"
#include "CollisionComponents.hpp"
#include "RenderComponents.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"
#include <cmath>

void WeaponRotationSystem::Update(entt::registry &registry)
{
    auto view = registry.view<const WeaponComponent, const PositionComponent, RenderEntityComponent>();

    for (auto [entity, weapon, position, render] : view.each())
    {
        // Si no hay enemigo objetivo válido, no rotar
        if (!registry.valid(weapon.targetEnemy))
        {
            continue;
        }

        // Obtener posición del enemigo
        auto *enemyPos = registry.try_get<PositionComponent>(weapon.targetEnemy);
        if (!enemyPos)
        {
            continue;
        }

        // Obtener hitbox del enemigo para calcular el centro
        auto *enemyHitbox = registry.try_get<RectangleHitboxComponent>(weapon.targetEnemy);

        // Calcular el centro del enemigo (igual que en AWeapon::CalculateDirection)
        float enemyCenterX = enemyPos->x;
        float enemyCenterY = enemyPos->y;

        if (enemyHitbox)
        {
            enemyCenterX += enemyHitbox->width * 0.5f;
            enemyCenterY += enemyHitbox->height * 0.5f;
        }

        // Calcular ángulo hacia el CENTRO del enemigo
        float dx = enemyCenterX - position.x;
        float dy = enemyCenterY - position.y;

        // atan2 devuelve el ángulo en radianes y los pasamos a grados
        float angleRadians = std::atan2(dy, dx);
        float angleDegrees = angleRadians * (180.0f / 3.14159265f);

        // Añadimos 45 grados porque el sprite del arma está orientado en diagonal
        angleDegrees += 45.0f;

        // Actualizar el ángulo de rotación
        render.angle = angleDegrees;
    }
}
