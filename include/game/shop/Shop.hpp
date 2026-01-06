#pragma once

#include "ShopSlot.hpp"
#include <array>

class Shop
{

public:
    constexpr static int MAX_ITEMS_SHOP = 5;
    Shop();
    ~Shop() = default;
    void reRoll();
    const std::array<TShopSlot, MAX_ITEMS_SHOP> &GetItemsShop() const;
    void AlternateBlockSlot(int index);
    bool IsSlotBlocked(int index) const;
    const Item *BuyItem(int index) const;

    // Pongo private abajo porque necesito la constante
private:
    std::array<TShopSlot, MAX_ITEMS_SHOP> shopPool;
};
