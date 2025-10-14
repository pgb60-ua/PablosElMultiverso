#pragma once
#include "Player.hpp"
#include "raylib.h"
#include <GameState.hpp>
#include <memory>

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
    std::unique_ptr<Player> player;
};
