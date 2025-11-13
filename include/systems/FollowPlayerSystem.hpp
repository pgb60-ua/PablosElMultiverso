#pragma once

#include <EnTT/entt.hpp>
extern "C"
{
#include <raylib.h>
}

class FollowEnemySystem
{
public:
    FollowEnemySystem() {};
    ~FollowEnemySystem() {};
    void Update(entt::registry &registry, float deltaTime);
};
