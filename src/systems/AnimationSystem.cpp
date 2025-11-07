#include "AnimationSystem.hpp"
#include "PlayerComponent.hpp"
#include "RenderComponents.hpp"
#include "SpriteLoaderManager.hpp"

void AnimationSystem::Update(entt::registry &registry, float deltaTime)
{
    UpdatePlayersAnimation(registry, deltaTime);
    UpdateEnemiesAnimation(registry, deltaTime);
}

void AnimationSystem::UpdatePlayersAnimation(entt::registry &registry, float deltaTime)
{
    auto view = registry.view<const PlayerComponent, RenderEntityComponent>();
    for (auto [entity, player, render] : view.each())
    {
        render.animation.timeAccumulator += deltaTime;

        if (render.animation.timeAccumulator >= render.animation.FRAME_DURATION)
        {
            render.animation.timeAccumulator = 0.0f;
            render.animation.frameIndex++;
            render.animation.frameIndex %=
                SpriteLoaderManager::GetInstance().GetSpriteSheet(player.type).spriteFrameCount;
        }
    }
};

void AnimationSystem::UpdateEnemiesAnimation(entt::registry &registry, float deltaTime) {}
