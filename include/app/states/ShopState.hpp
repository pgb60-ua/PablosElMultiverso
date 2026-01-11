#pragma once

#include "Player.hpp"
#include "Shop.hpp"
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
    Player *player;
    Shop shop;
    int selectedItem = 0;
    bool willBuy = false;
    bool willAlternateBlock = false;
    bool willReroll = false;
    constexpr static int PADDING = 100;
    constexpr static int REROLL_COST = 5;
    void NextSelectedItem(int direction);

public:
    ShopState(Player *player);
    ~ShopState();

    void init() override;
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() {};
    void resume() {};
};
