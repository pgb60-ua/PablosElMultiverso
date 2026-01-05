#pragma once

#include <GameState.hpp>
#include <memory>

extern "C"
{
#include "raylib.h"
}

class ShopState : public GameState
{
private:
    bool passRound = false;

public:
    ShopState();
    ~ShopState();

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() {};
    void resume() {};
};
