#include "ItemsFactory.hpp"
#include "DataFileManager.hpp"
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
    for (int i = 0; i < 10; i++)
    {
        ItemData itemData = dm.GetItemData(ITEM_TYPE::ITEM1);
        auto item = std::make_unique<Item>(itemData.name, itemData.description, itemData.stats, itemData.rarity,
                                           itemData.price);
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
            weight = 30.0;
            break; // 30%
        case ItemRarity::Rare:
            weight = 15.0;
            break; // 15%
        case ItemRarity::Epic:
            weight = 5.0;
            break; // 5%
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
