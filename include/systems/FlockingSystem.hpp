#pragma once

#include <EnTT/entt.hpp>
extern "C"
{
#include <raylib.h>
}
class FlockingSystem
{
public:
    FlockingSystem() {};
    ~FlockingSystem() {};
    void Update(entt::registry &registry, float deltaTime);
};
