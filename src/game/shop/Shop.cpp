#include "Shop.hpp"
#include "Item.hpp"
#include "ItemsFactory.hpp"
#include "ShopSlot.hpp"
#include <spdlog/spdlog.h>

Shop::Shop()
{
    std::vector<const Item *> randomItems = ItemsFactory::GetInstance().GetRandomItems(MAX_ITEMS_SHOP);

    for (size_t i = 0; i < randomItems.size() && i < MAX_ITEMS_SHOP; ++i)
    {
        shopPool[i] = {randomItems[i], false, false};
    }

    // Si no hay suficientes items, llenar el resto con nullptr
    for (size_t i = randomItems.size(); i < MAX_ITEMS_SHOP; ++i)
    {
        shopPool[i] = {nullptr, false, false};
    }
};
Shop::~Shop() {};

const std::array<TShopSlot, Shop::MAX_ITEMS_SHOP> &Shop::GetItemsShop() const { return shopPool; };

void Shop::reRoll()
{
    std::vector<const Item *> randomItems = ItemsFactory::GetInstance().GetRandomItems(MAX_ITEMS_SHOP);
    size_t randomIndex = 0;

    for (size_t i = 0; i < MAX_ITEMS_SHOP; ++i)
    {
        // Solo cambiar items que NO estén bloqueados
        if (!shopPool[i].isBlocked && randomIndex < randomItems.size())
        {
            shopPool[i].item = randomItems[randomIndex];
            shopPool[i].isBuyed = false; // Resetear estado de compra
            randomIndex++;
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
