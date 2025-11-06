#include "MovementSystem.hpp"
#include "InputComponent.hpp"
#include "TransformComponent.hpp"

void MovementSystem::Update(entt::registry &registry, float deltaTime)
{
    auto view = registry.view<const InputComponent, PositionComponent, const MovementSpeedComponent>();

    for (auto [entity, input, position, speed] : view.each())
    {
        if (input.direction.x == 0 && input.direction.x == 0)
        {
            continue; // No hay que modificar posiciones
        }

        // Normalizar para evitar velocidad extra en diagonal
        float length = sqrtf(input.direction.x * input.direction.x + input.direction.y * input.direction.y);

        float normalizedX = input.direction.x / length;
        float normalizedY = input.direction.y / length;

        position.x = normalizedX * speed.movementSpeed * deltaTime;
        position.y = normalizedY * speed.movementSpeed * deltaTime;
    }
}
