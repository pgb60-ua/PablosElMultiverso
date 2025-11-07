#pragma once
#include <EnTT/entt.hpp>

class RenderSystem
{
public:
    RenderSystem() {};
    ~RenderSystem() {};
    void Update(entt::registry &registry);

private:
    void UpdateEntities(entt::registry &registry);
    void UpdateItems(entt::registry &registry);
    void UpdateBullets(entt::registry &registry);
};
