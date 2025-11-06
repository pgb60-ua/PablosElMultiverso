#pragma once
#include <EnTT/entt.hpp>

class MovementSystem
{
public:
    MovementSystem();
    ~MovementSystem();
    void Update(entt::registry &registry, float deltaTime);
};
