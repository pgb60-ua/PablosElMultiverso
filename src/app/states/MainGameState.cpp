#include "CollisionComponents.hpp"
#include "InputComponent.hpp"
#include "InputSystem.hpp"
#include "MovementSystem.hpp"
#include "PlayerComponent.hpp"
#include "RenderComponent.hpp"
#include "RenderSystem.hpp"
#include "TransformComponent.hpp"
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
    registry.emplace<RenderComponent>(player, 0.f);
    registry.emplace<PlayerComponent>(player, "Prueba", 1);
    registry.emplace<MovementSpeedComponent>(player, 100);
    // Para eliminar, esto es prueba
    registry.emplace<RectangleHitboxComponent>(player, 20.0f, 20.0f, 0.f, 0.f);
}

void MainGameState::handleInput() { inputSystem.Update(registry); }

void MainGameState::update(float deltaTime) { movementSystem.Update(registry, deltaTime); }

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
    for (const auto entity : registry.view<RenderComponent>())
    {
        registry.destroy(entity);
    }
}
