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

void Shop::BlockSlot(int index)
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to block an item out of range");
        return;
    }
    shopPool[index].isBlocked = true;
}

void Shop::UnblockSlot(int index)
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to unblock an item out of range");
        return;
    }
    shopPool[index].isBlocked = false;
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

const Item *Shop::BuyItem(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to buy an item out of range");
        return nullptr; // Compra fallida
    }

    return shopPool[index].item; // Compra exitosa
}
