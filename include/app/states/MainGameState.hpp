#pragma once
#include "Player.hpp"
#include "AEnemy.hpp"
#include <GameState.hpp>
#include <memory>

extern "C"
{
    #include "raylib.h"
}

class MainGameState : public GameState
{
public:
    MainGameState();
    ~MainGameState() = default;

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() {};
    void resume() {};

private:
    char entered_key;
    Vector2 direction;
    Vector2 direction2;
    std::vector<std::unique_ptr<Player>> players;
    std::vector<std::unique_ptr<AEnemy>> enemies;
};
