#include "AccelerationComponent.hpp"
#include "AnimationSystem.hpp"
#include "CollisionComponents.hpp"
#include "EnemyComponent.hpp"
#include "EntityComponent.hpp"
#include "FlockingComponent.hpp"
#include "FollowPlayerComponent.hpp"
#include "InputComponent.hpp"
#include "InputSystem.hpp"
#include "MovementSystem.hpp"
#include "NormalTypeEnemiesComponents.hpp"
#include "PlayerComponent.hpp"
#include "RangeWeaponComponent.hpp"
#include "RenderComponents.hpp"
#include "RenderSystem.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
#include "TransformComponent.hpp"
#include "Types.hpp"
#include "WeaponComponent.hpp"
#include "raymath.h"
#include <MainGameState.hpp>

MainGameState::MainGameState() {}

void MainGameState::init()
{
    auto player = registry.create();
    registry.emplace<InputComponent>(player, Vector2{0, 0}, 0);
    registry.emplace<PositionComponent>(player, 400.0f, 300.0f);
    registry.emplace<RenderEntityComponent>(player, 0.f);
    registry.emplace<PlayerComponent>(player, "Prueba", 1, PLAYER_TYPE::RANGE);
    registry.emplace<MovementSpeedComponent>(player, 100.f);
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::RANGE);
    // Para eliminar, esto es prueba (porque de momento lo uso para render)
    registry.emplace<RectangleHitboxComponent>(player, sheet.frames.begin()->width, sheet.frames.begin()->height, 0.f,
                                               0.f);
    registry.emplace<EntityComponent>(player, ALIVE);

    auto weaponPlayer1 = registry.create();
    registry.emplace<PositionComponent>(weaponPlayer1, 200.0f, 200.0f);
    registry.emplace<WeaponComponent>(weaponPlayer1, player, sheet.frames.begin()->width, sheet.frames.begin()->height);
    registry.emplace<RangeWeaponComponent>(weaponPlayer1, WEAPON_TYPE::WING);
    const SpriteSheet &sheetWeapon = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::WING);
    registry.emplace<RectangleHitboxComponent>(weaponPlayer1, sheetWeapon.frames.begin()->width,
                                               sheet.frames.begin()->height, 0.f, 0.f);
    registry.emplace<RenderEntityComponent>(weaponPlayer1, 0.f);

    auto player2 = registry.create();
    registry.emplace<InputComponent>(player2, Vector2{0, 0}, 0);
    registry.emplace<PositionComponent>(player2, 200.0f, 300.0f);
    registry.emplace<RenderEntityComponent>(player2, 0.f);
    registry.emplace<PlayerComponent>(player2, "Prueba", 2, PLAYER_TYPE::MAGE);
    registry.emplace<MovementSpeedComponent>(player2, 100.f);
    const SpriteSheet &sheet2 = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::MAGE);
    // Para eliminar, esto es prueba (porque de momento lo uso para render)
    registry.emplace<RectangleHitboxComponent>(player2, sheet2.frames.begin()->width, sheet2.frames.begin()->height,
                                               0.f, 0.f);
    registry.emplace<EntityComponent>(player2, ALIVE);

    // Enemigo
    auto enemy = registry.create();
    registry.emplace<MovementSpeedComponent>(enemy, 60.0f);
    registry.emplace<PositionComponent>(enemy, 200.0f, 200.0f);
    const SpriteSheet &enemySheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);
    registry.emplace<RectangleHitboxComponent>(enemy, enemySheet.frames.begin()->width,
                                               enemySheet.frames.begin()->height, 0.f, 0.f);
    registry.emplace<RenderEntityComponent>(enemy, 0.f);
    registry.emplace<EnemyComponent>(enemy, ENEMY_TYPE::ZOMBIE);
    registry.emplace<ZombieComponent>(enemy);
    registry.emplace<EntityComponent>(enemy, ALIVE);
    registry.emplace<FlockingComponent>(enemy, 250.0f, 80.0f, 1.5f, 1.0f, 0.5f, 2.0f, 2.0f);
    registry.emplace<FollowPlayerComponent>(enemy, entt::null);
    // Velocidad inicial aleatoria
    Vector2 randomVel = {((float)rand() / RAND_MAX) * 2.0f - 1.0f, ((float)rand() / RAND_MAX) * 2.0f - 1.0f};
    if (Vector2Length(randomVel) > 0.0f)
    {
        randomVel = Vector2Scale(Vector2Normalize(randomVel), 100.0f * 0.5f);
    }
    registry.emplace<VelocityComponent>(enemy, randomVel);
    registry.emplace<AccelerationComponent>(enemy, Vector2Zero());

    // Enemigo2
    auto enemy2 = registry.create();
    registry.emplace<MovementSpeedComponent>(enemy2, 60.0f);
    registry.emplace<PositionComponent>(enemy2, 100.0f, 200.0f);
    const SpriteSheet &enemySheet2 = SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);
    registry.emplace<RectangleHitboxComponent>(enemy2, enemySheet2.frames.begin()->width,
                                               enemySheet.frames.begin()->height, 0.f, 0.f);
    registry.emplace<RenderEntityComponent>(enemy2, 0.f);
    registry.emplace<EnemyComponent>(enemy2, ENEMY_TYPE::ZOMBIE);
    registry.emplace<ZombieComponent>(enemy2);
    registry.emplace<EntityComponent>(enemy2, ALIVE);
    registry.emplace<FlockingComponent>(enemy2, 250.0f, 80.0f, 1.5f, 1.0f, 0.5f, 2.0f, 2.0f);
    registry.emplace<FollowPlayerComponent>(enemy2, entt::null);
    registry.emplace<VelocityComponent>(enemy2, randomVel);
    registry.emplace<AccelerationComponent>(enemy2, Vector2Zero());
}

void MainGameState::handleInput() { inputSystem.Update(registry); }

void MainGameState::update(float deltaTime)
{
    updateClosestPlayerSystem.Update(registry, deltaTime); // 1. Actualizar targets
    flockingSystem.Update(registry);    // 2. Calcular fuerzas de flocking -> acumulan en acceleration
    followEnemySystem.Update(registry); // 3. Calcular fuerza hacia jugador -> suma a acceleration
    accelerationIntegrationSystem.Update(registry, deltaTime); // 4. Aplicar acceleration a velocity (con deltaTime)
    velocityLimiterSystem.Update(registry);                    // 5. Limitar velocity máxima
    movementSystem.Update(registry, deltaTime);                // 6. Aplicar velocity a position
    animationSystem.Update(registry, deltaTime);
    weaponPositionSystem.Update(registry);
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
