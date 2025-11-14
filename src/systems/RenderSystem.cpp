#include "RenderSystem.hpp"
#include "EnemyComponent.hpp"
#include "EntityComponent.hpp"
#include "PlayerComponent.hpp"
#include "ProjectileComponent.hpp"
#include "RangeWeaponComponent.hpp"
#include "RenderComponents.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
#include "TransformComponent.hpp"
#include "raylib.h"

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
    auto players =
        registry.view<const PlayerComponent, RenderEntityComponent, const PositionComponent, const EntityComponent>();
    for (auto [entity, player, render, position, entComponent] : players.each())
    {
        // Si el jugador esta muerto o en la tienda no tiene que renderizarse
        if (entComponent.state == DEAD || entComponent.state == SHOP)
            continue;

        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(player.type);
        if (sheet.frames.empty())
            continue;

        AuxUpdateEntities(sheet, render, position);
    }

    auto enemies =
        registry.view<const EnemyComponent, RenderEntityComponent, const PositionComponent, const EntityComponent>();
    for (auto [entity, enemy, render, position, entComponent] : enemies.each())
    {
        // Si el enemigo esta muerto o en la tienda no tiene que renderizarse
        if (entComponent.state == DEAD || entComponent.state == SHOP)
            continue;

        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(enemy.type);
        if (sheet.frames.empty())
            continue;

        AuxUpdateEntities(sheet, render, position);
    }
}

void RenderSystem::UpdateItems(entt::registry &registry)
{
    auto view = registry.view<const PositionComponent, RenderEntityComponent, const RangeWeaponComponent>();
    // TODO Comprobar que no se muestren armas cuando jugador muerto
    for (auto [entity, position, render, range] : view.each())
    {
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(range.type);
        if (sheet.frames.empty())
            continue;
        AuxUpdateEntities(sheet, render, position);
    }
}

void RenderSystem::UpdateBullets(entt::registry &registry)
{
    auto projectiles = registry.view<PositionComponent, RenderEntityComponent, const ProjectileComponent>();

    for (auto [entity, position, render, projectile] : projectiles.each())
    {
        if (!projectile.active)
            continue;
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(projectile.type);
        if (sheet.frames.empty())
            continue;
        AuxUpdateEntities(sheet, render, position);
    }
}

void RenderSystem::AuxUpdateEntities(const SpriteSheet &sheet, RenderEntityComponent &render,
                                     const PositionComponent &position)
{
    render.animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[render.animation.frameIndex];

    if (render.animation.flipped)
    {
        src.width *= -1.0f;
    }

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

    Rectangle dest = {position.x, position.y, src.width, src.height};

    DrawTexturePro(sheet.texture, src, dest, origin, render.angle, render.animation.color);
}
