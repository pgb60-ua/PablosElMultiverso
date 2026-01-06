#include "Shop.hpp"
#include "Item.hpp"
#include "ShopSlot.hpp"
#include "raylib.h"
#include <spdlog/spdlog.h>

Shop::Shop()
    : testItem("name", "description",
               Stats(OffensiveStats{10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f},
                     DefensiveStats{0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}),
               ItemRarity::Common, 10)
{
    // De forma provisional
    texture = LoadTexture("assets/sprites/shop/items/Zombiesaur_64.png");
    testItem.SetIcon(&texture);
    TShopSlot slot1 = {&testItem, false, false};
    shopPool.fill(slot1);
};

Shop::~Shop() { UnloadTexture(texture); };

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

const Item *Shop::BuyItem(int index)
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to buy an item out of range");
        return nullptr; // Compra fallida
    }
    if (shopPool[index].isBuyed)
    {
        return nullptr;
    }

    // Validacion mas que nada por bugs porque deberian ser siempre validos los items
    const Item *item = shopPool[index].item;
    if (item == nullptr)
        return nullptr;

    shopPool[index].isBuyed = true;
    return shopPool[index].item; // Compra exitosa
}

bool Shop::IsSlotBuyed(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to acces if an item is blocked out of range");
        return false;
    }
    return shopPool[index].isBuyed;
}
