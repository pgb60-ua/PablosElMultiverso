#include "RenderSystem.hpp"
#include "CollisionComponents.hpp"
#include "RenderComponent.hpp"
#include "TransformComponent.hpp"
#include "raylib.h"

void RenderSystem::Update(entt::registry &registry)
{
    auto view = registry.view<const RenderComponent, const PositionComponent, const RectangleHitboxComponent>();
    for (auto [entity, render, position, rectangle] : view.each())
    {
        DrawRectangle(position.x, position.y, rectangle.width, rectangle.height, RED);
    }
}
