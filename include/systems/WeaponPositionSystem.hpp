#pragma once

#include <EnTT/entt.hpp>

class WeaponPositionSystem
{
public:
    WeaponPositionSystem() {};
    ~WeaponPositionSystem() {};
    void Update(entt::registry &registry);
};
