#include "RenderSystem.hpp"
#include "CollisionComponents.hpp"
#include "InputComponent.hpp"
#include "PlayerComponent.hpp"
#include "RenderComponents.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
#include "TransformComponent.hpp"

void RenderSystem::Update(entt::registry &registry)
{
    // Renderizamos los jugadores y enemigos
    UpdateEntities(registry);
    // Renderizamos los items / armas
    UpdateItems(registry);
    // Renderizamos las balas
    UpdateBullets(registry);
}

void RenderSystem::UpdateEntities(entt::registry &registry)
{
    auto view = registry.view<const PlayerComponent, RenderEntityComponent, const PositionComponent,
                              const InputComponent, const RectangleHitboxComponent>();
    for (auto [entity, player, render, position, input, hitbox] : view.each())
    {
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(player.type);
        if (sheet.frames.empty())
            continue;
        render.animation.frameIndex %= sheet.spriteFrameCount;

        Rectangle src = sheet.frames[render.animation.frameIndex];
        if (input.direction.x != 0)
        {
            render.animation.flipped = (input.direction.x < 0);
        }
        if (render.animation.flipped)
        {
            src.width *= -1.0f;
        }

        Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                          src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

        Rectangle dest = {position.x, position.y, src.width, src.height};

        // Para hitbox visual - pruebas
        float hitboxX = position.x - (hitbox.width * 0.5f) + hitbox.offsetX;
        float hitboxY = position.y - (hitbox.height * 0.5f) + hitbox.offsetY;

        DrawTexturePro(sheet.texture, src, dest, origin, 0, render.animation.color);
        DrawRectangleLines(hitboxX, hitboxY, hitbox.width, hitbox.height, RED);
        DrawCircle(position.x, position.y, 2, GREEN);
    }
}

void RenderSystem::UpdateItems(entt::registry &registry) {}

void RenderSystem::UpdateBullets(entt::registry &registry) {}
