#include "Shop.hpp"
#include "Item.hpp"
#include "ItemsFactory.hpp"
#include "ShopSlot.hpp"
#include "Types.hpp"
#include <random>
#include <spdlog/spdlog.h>

Shop::Shop()
{
    std::vector<const Item *> randomItems = ItemsFactory::GetInstance().GetRandomItems(MAX_ITEMS_SHOP);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> levelDist(1, 4); // Niveles entre 1 y 4

    for (size_t i = 0; i < randomItems.size() && i < MAX_ITEMS_SHOP; ++i)
    {
        // Generar nivel aleatorio para armas
        int weaponLevel = 1;
        if (randomItems[i] != nullptr)
        {
            ITEM_TYPE itemType = randomItems[i]->GetType();
            bool isWeapon = (itemType >= ITEM_TYPE::WEAPON_AXE && itemType <= ITEM_TYPE::WEAPON_WING);
            if (isWeapon)
            {
                weaponLevel = levelDist(gen);
            }
        }

        shopPool[i] = {randomItems[i], false, false, weaponLevel};
    }

    // Si no hay suficientes items, llenar el resto con nullptr
    for (size_t i = randomItems.size(); i < MAX_ITEMS_SHOP; ++i)
    {
        shopPool[i] = {nullptr, false, false, 1};
    }
};
Shop::~Shop() {};

const std::array<TShopSlot, Shop::MAX_ITEMS_SHOP> &Shop::GetItemsShop() const { return shopPool; };

void Shop::reRoll()
{
    std::vector<const Item *> randomItems = ItemsFactory::GetInstance().GetRandomItems(MAX_ITEMS_SHOP);
    size_t randomIndex = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> levelDist(1, 4); // Niveles entre 1 y 4

    for (size_t i = 0; i < MAX_ITEMS_SHOP; ++i)
    {
        // Solo cambiar items que NO estén bloqueados
        if (!shopPool[i].isBlocked && randomIndex < randomItems.size())
        {
            shopPool[i].item = randomItems[randomIndex];
            shopPool[i].isBuyed = false; // Resetear estado de compra

            // Generar nivel aleatorio para armas
            int weaponLevel = 1;
            if (randomItems[randomIndex] != nullptr)
            {
                ITEM_TYPE itemType = randomItems[randomIndex]->GetType();
                bool isWeapon = (itemType >= ITEM_TYPE::WEAPON_AXE && itemType <= ITEM_TYPE::WEAPON_WING);
                if (isWeapon)
                {
                    weaponLevel = levelDist(gen);
                }
            }
            shopPool[i].weaponLevel = weaponLevel;

            randomIndex++;
        }
    }
}

bool Shop::IsSlotBlocked(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to access if an item is blocked out of range");
        return false;
    }
    return shopPool[index].isBlocked;
}

void Shop::AlternateBlockSlot(int index)
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to access an item out of range");
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
    shopPool[index].isBlocked = false;
    return shopPool[index].item; // Compra exitosa
}

bool Shop::IsSlotBuyed(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to access if an item is buyed out of range");
        return false;
    }
    return shopPool[index].isBuyed;
}
