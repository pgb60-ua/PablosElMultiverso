#pragma once

#include <entt/entt.hpp>

class GetClosestEnemySystem
{
private:
    float timeSinceLastUpdate = 0.0f;
    const float UPDATE_INTERVAL = 0.2f; // Actualizar cada 0.2 segundos (optimización)

public:
    GetClosestEnemySystem() {};
    ~GetClosestEnemySystem() {};
    void Update(entt::registry &registry, float deltaTime);
};
