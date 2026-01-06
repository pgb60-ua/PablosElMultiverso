#pragma once

#include "ShopSlot.hpp"
#include <array>

class Shop
{
private:
    constexpr static int MAX_ITEMS_SHOP = 5;
    std::array<TShopSlot, MAX_ITEMS_SHOP> shopPool;

public:
    Shop();
    ~Shop() = default;
    void reRoll();
    const std::array<TShopSlot, MAX_ITEMS_SHOP> &GetItemsShop() const;
    void BlockSlot(int index);
    void UnblockSlot(int index);
    bool IsSlotBlocked(int index) const;
    const Item *BuyItem(int index) const;
};
