#pragma once

#include <EnTT/entt.hpp>
class AnimationSystem
{
public:
    AnimationSystem() {};
    ~AnimationSystem() {};
    void Update(entt::registry &registry, float deltaTime);

private:
    void UpdatePlayersAnimation(entt::registry &registry, float deltaTime);
    void UpdateEnemiesAnimation(entt::registry &registry, float deltaTime);
};
