#pragma once

#include <entt/entt.hpp>

class WeaponRotationSystem
{
public:
    WeaponRotationSystem() {};
    ~WeaponRotationSystem() {};
    void Update(entt::registry &registry);
};
