#pragma once

#include <entt/entt.hpp>

class AccelerationIntegrationSystem
{
public:
    void Update(entt::registry &registry, float deltaTime);
};
