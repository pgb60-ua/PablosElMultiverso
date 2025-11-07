#include "WeaponPositionSystem.hpp"
#include "TransformComponent.hpp"
#include "WeaponComponent.hpp"

void WeaponPositionSystem::Update(entt::registry &registry)
{
    auto view = registry.view<const WeaponComponent, PositionComponent>();

    for (auto [entity, weapon, position] : view.each())
    {

        if (auto *player = registry.try_get<const PositionComponent>(weapon.playerOwnerEntity))
        {
            position.x = weapon.offsetX + player->x;
            position.y = weapon.offsetY + player->y;
        }
    }
}
