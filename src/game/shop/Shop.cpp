#include "Shop.hpp"
#include "ShopSlot.hpp"
#include <spdlog/spdlog.h>

Shop::Shop() {};

const std::array<TShopSlot, Shop::MAX_ITEMS_SHOP> &Shop::GetItemsShop() const { return shopPool; };

void Shop::reRoll()
{
    for (TShopSlot slot : shopPool)
    {
        if (!slot.isBlocked)
        {
            // TODO : pedir item para cambiarlo de toda la pool cargada
        }
    }
}

bool Shop::IsSlotBlocked(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to acces if an item is blocked out of range");
        return false;
    }
    return shopPool[index].isBlocked;
}

void Shop::AlternateBlockSlot(int index)
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to acces an item out of range");
        return;
    }
    shopPool[index].isBlocked = !shopPool[index].isBlocked;
};

const Item *Shop::BuyItem(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to buy an item out of range");
        return nullptr; // Compra fallida
    }

    return shopPool[index].item; // Compra exitosa
}
