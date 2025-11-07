#include "AnimationSystem.hpp"
#include "CollisionComponents.hpp"
#include "EntityComponent.hpp"
#include "InputComponent.hpp"
#include "InputSystem.hpp"
#include "MovementSystem.hpp"
#include "PlayerComponent.hpp"
#include "RenderComponents.hpp"
#include "RenderSystem.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
#include "TransformComponent.hpp"
#include "Types.hpp"
#include <MainGameState.hpp>

extern "C"
{
#include <raylib.h>
}

MainGameState::MainGameState() {}

void MainGameState::init()
{
    auto player = registry.create();
    registry.emplace<InputComponent>(player, Vector2{0, 0}, 0);
    registry.emplace<PositionComponent>(player, 400.0f, 300.0f);
    registry.emplace<RenderEntityComponent>(player);
    registry.emplace<PlayerComponent>(player, "Prueba", false, PLAYER_TYPE::RANGE);
    registry.emplace<MovementSpeedComponent>(player, 100.f);
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::RANGE);
    // Para eliminar, esto es prueba (porque de momento lo uso para render)
    registry.emplace<RectangleHitboxComponent>(player, sheet.frames.begin()->width, sheet.frames.begin()->height, 0.f,
                                               0.f);
    registry.emplace<EntityComponent>(player, ALIVE);

    auto player2 = registry.create();
    registry.emplace<InputComponent>(player2, Vector2{0, 0}, 0);
    registry.emplace<PositionComponent>(player2, 200.0f, 300.0f);
    registry.emplace<RenderEntityComponent>(player2);
    registry.emplace<PlayerComponent>(player2, "Prueba", false, PLAYER_TYPE::MAGE);
    registry.emplace<MovementSpeedComponent>(player2, 100.f);
    const SpriteSheet &sheet2 = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::MAGE);
    // Para eliminar, esto es prueba (porque de momento lo uso para render)
    registry.emplace<RectangleHitboxComponent>(player2, sheet2.frames.begin()->width, sheet2.frames.begin()->height,
                                               0.f, 0.f);
    registry.emplace<EntityComponent>(player2, ALIVE);
}

void MainGameState::handleInput() { inputSystem.Update(registry); }

void MainGameState::update(float deltaTime)
{
    movementSystem.Update(registry, deltaTime);
    animationSystem.Update(registry, deltaTime);
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Pablos El Multiverso", 10, 10, 20, LIGHTGRAY);
    renderSystem.Update(registry);
    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
}

MainGameState::~MainGameState()
{
    for (const auto entity : registry.view<PositionComponent>())
    {
        registry.destroy(entity);
    }
}
