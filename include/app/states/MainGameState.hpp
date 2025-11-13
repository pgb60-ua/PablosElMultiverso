#pragma once
#include "AnimationSystem.hpp"
#include "FlockingSystem.hpp"
#include "FollowPlayerSystem.hpp"
#include "InputSystem.hpp"
#include "MovementSystem.hpp"
#include "RenderSystem.hpp"
#include "UpdateClosestPlayerSystem.hpp"
#include "WeaponPositionSystem.hpp"
#include <EnTT/entt.hpp>
#include <GameState.hpp>

extern "C"
{
#include "raylib.h"
}

class MainGameState : public GameState
{
public:
    MainGameState();
    ~MainGameState();

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() {};
    void resume() {};

private:
    entt::registry registry;
    InputSystem inputSystem;
    RenderSystem renderSystem;
    MovementSystem movementSystem;
    AnimationSystem animationSystem;
    WeaponPositionSystem weaponPositionSystem;
    UpdateClosestPlayerSystem updateClosestPlayerSystem;
    FlockingSystem flockingSystem;
    FollowEnemySystem followEnemySystem;
};
