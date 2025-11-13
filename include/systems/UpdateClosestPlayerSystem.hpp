#pragma once

#include <EnTT/entt.hpp>

class UpdateClosestPlayerSystem
{
private:
    float timeSinceLastUpdate = 0.0f;
    const float UPDATE_INTERVAL = 0.5f; // Tiempo cada cual se updatea
public:
    UpdateClosestPlayerSystem() {};
    ~UpdateClosestPlayerSystem() {};
    void Update(entt::registry &registry, float deltaTime);
};
