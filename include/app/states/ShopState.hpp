#pragma once

#include "Player.hpp"
#include "Shop.hpp"
#include <GameState.hpp>

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
    // 0, 1, 2, 3, 4 - Items de compra
    // 5, 6, 7, 8 - Inventario de armas
    int selectedItem = 0;
    bool willBuy = false;
    bool willAlternateBlock = false;
    bool willReroll = false;
    bool willFuse = false;
    bool willSell = false;
    constexpr static int PADDING = 100;
    constexpr static int REROLL_COST = 5;
    void NextSelectedItem(int direction);
    /// @brief devuelve a que arma salto del inventario
    void NextWeaponSelected(int direction);
    int CalculateWeaponSellPrice(int weaponIndex);

public:
    ShopState(Player *player);
    ~ShopState();

    void init() override {};
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() override {};
    void resume() override {};
};
