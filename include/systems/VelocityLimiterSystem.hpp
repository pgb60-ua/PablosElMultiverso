#pragma once

#include <EnTT/entt.hpp>

class VelocityLimiterSystem
{
public:
    VelocityLimiterSystem() {};
    ~VelocityLimiterSystem() {};
    void Update(entt::registry &registry);
};
