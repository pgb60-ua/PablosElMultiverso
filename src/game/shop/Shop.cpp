#include "Shop.hpp"
#include "Item.hpp"
#include "ItemsFactory.hpp"
#include "ShopSlot.hpp"
#include "Types.hpp"
#include <random>
#include <spdlog/spdlog.h>

// Generador estático para evitar reinicialización
static std::random_device rd;
static std::mt19937 generator(rd());

bool Shop::IsValidIndex(int index) const
{
    if (index < 0 || index >= MAX_ITEMS_SHOP)
    {
        spdlog::warn("Trying to access shop slot out of range: index={}", index);
        return false;
    }
    return true;
}

int Shop::GenerateWeaponLevel(const Item *item)
{
    if (item == nullptr)
    {
        return 1;
    }

    ITEM_TYPE itemType = item->GetType();
    bool isWeapon = (itemType >= ITEM_TYPE::WEAPON_AXE && itemType <= ITEM_TYPE::WEAPON_WING);

    if (!isWeapon)
    {
        return 1;
    }

    // Generar nivel con distribución de probabilidad
    std::discrete_distribution<> levelDist(std::begin(WEAPON_LEVEL_WEIGHTS), std::end(WEAPON_LEVEL_WEIGHTS));
    return levelDist(generator) + 1; // +1 porque devuelve 0-3 y queremos 1-4
}

Shop::Shop()
{
    std::vector<const Item *> randomItems = ItemsFactory::GetInstance().GetRandomItems(MAX_ITEMS_SHOP);

    for (size_t i = 0; i < randomItems.size() && i < MAX_ITEMS_SHOP; ++i)
    {
        int weaponLevel = GenerateWeaponLevel(randomItems[i]);
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

    for (size_t i = 0; i < MAX_ITEMS_SHOP; ++i)
    {
        // Solo cambiar items que NO estén bloqueados
        if (!shopPool[i].isBlocked && randomIndex < randomItems.size())
        {
            shopPool[i].item = randomItems[randomIndex];
            shopPool[i].isBuyed = false; // Resetear estado de compra
            shopPool[i].weaponLevel = GenerateWeaponLevel(randomItems[randomIndex]);
            randomIndex++;
        }
    }
}

bool Shop::IsSlotBlocked(int index) const
{
    if (!IsValidIndex(index))
        return false;

    return shopPool[index].isBlocked;
}

void Shop::AlternateBlockSlot(int index)
{
    if (!IsValidIndex(index))
        return;

    shopPool[index].isBlocked = !shopPool[index].isBlocked;
}

const Item *Shop::BuyItem(int index)
{
    if (!IsValidIndex(index))
        return nullptr;

    if (shopPool[index].isBuyed)
        return nullptr;

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
    if (!IsValidIndex(index))
        return false;

    return shopPool[index].isBuyed;
}
