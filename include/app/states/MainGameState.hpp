#pragma once
#include "AEnemy.hpp"
#include "Player.hpp"
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
    ~MainGameState();

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
    std::vector<Player> players;
    std::vector<AEnemy *> enemies;
    AWeapon *currentWeapon;
};
