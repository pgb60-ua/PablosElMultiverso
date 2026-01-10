#include "ItemsFactory.hpp"
#include "DataFileManager.hpp"
#include "Types.hpp"
#include "spdlog/spdlog.h"
#include <memory>
#include <random>        // para std::random_device, std::mt19937
#include <unordered_set> // para evitar repeticiones
#include <vector>

ItemsFactory::ItemsFactory() {};

ItemsFactory::~ItemsFactory() {}

void ItemsFactory::LoadAllItems()
{
    DataFileManager &dm = DataFileManager::GetInstance();

    // Iterar desde el primer item hasta el último
    for (int i = static_cast<int>(ITEM_TYPE::FIREBALL); i <= static_cast<int>(ITEM_TYPE::RAW_MEAT); ++i)
    {
        ITEM_TYPE type = static_cast<ITEM_TYPE>(i);
        ItemData itemData = dm.GetItemData(type);
        auto item = std::make_unique<Item>(itemData.name, itemData.description, itemData.stats, itemData.rarity,
                                           itemData.price);
        item->SetType(type);
        allItems.push_back(std::move(item));
    }

    spdlog::info("ItemsFactory: Loaded {} items", allItems.size());
};

std::vector<const Item *> ItemsFactory::GetRandomItems(int count)
{
    std::vector<const Item *> result;

    if (allItems.empty() || count <= 0)
        return result;

    // Crear distribución con pesos según rareza
    std::vector<double> weights;
    weights.reserve(allItems.size());

    for (const auto &item : allItems)
    {
        double weight = 1.0;
        switch (item->GetItemRarity())
        {
        case ItemRarity::Common:
            weight = 50.0;
            break; // 50% probabilidad
        case ItemRarity::Uncommon:
            weight = 25.0;
            break; // 25%
        case ItemRarity::Rare:
            weight = 15.0;
            break; // 15%
        case ItemRarity::Epic:
            weight = 7.0;
            break; // 7%
        case ItemRarity::Legendary:
            weight = 3.0;
            break; // 3%
        }
        weights.push_back(weight);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    // Seleccionar 'count' items sin repetición
    std::unordered_set<int> selectedIndices;
    while (selectedIndices.size() < static_cast<size_t>(count) && selectedIndices.size() < allItems.size())
    {
        int index = dist(gen);
        selectedIndices.insert(index);
    }

    for (int index : selectedIndices)
    {
        result.push_back(allItems[index].get());
    }

    return result;
}
