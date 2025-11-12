#pragma once
#include <EnTT/entt.hpp>

#include "RenderComponents.hpp"
#include "SpriteSheet.hpp"
#include "TransformComponent.hpp"

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
    void AuxUpdateEntities(const SpriteSheet &sheet, RenderEntityComponent &render, const PositionComponent &position);
};
