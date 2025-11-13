#include "VelocityLimiterSystem.hpp"
#include "TransformComponent.hpp"
#include "raymath.h"

void VelocityLimiterSystem::Update(entt::registry &registry)
{
    auto view = registry.view<VelocityComponent, const MovementSpeedComponent>();

    for (auto [entity, velocity, speed] : view.each())
    {
        float currentSpeed = Vector2Length(velocity.velocity);
        if (currentSpeed > speed.movementSpeed)
        {
            velocity.velocity = Vector2Scale(Vector2Normalize(velocity.velocity), speed.movementSpeed);
        }
    }
}
