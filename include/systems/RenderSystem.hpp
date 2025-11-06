#pragma once
#include <EnTT/entt.hpp>

class RenderSystem
{
public:
    RenderSystem() {};
    ~RenderSystem() {};
    void Update(entt::registry &registry);
};
