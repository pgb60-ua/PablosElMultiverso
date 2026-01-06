#pragma once
#include "Stats.hpp"
#include <string>
extern "C"
{
#include <raylib.h>
}

enum class ItemRarity
{
    Common,
    Uncommon,
    Rare,
    Epic
};

class Item
{
protected:
    /// @brief nombre del objeto
    std::string name;
    /// @brief  Descripción del objeto
    std::string description;
    /// @brief Estadisticas del objeto
    Stats stats;
    /// @brief Rareza del objeto
    ItemRarity itemRarity;
    /// @brief Precio del item
    int price;

public:
    /// @brief Constructor de la clase Item

    Item(std::string name, std::string description, Stats stats, ItemRarity itemRarity, int price);

    /// @brief Getter del nombre del objeto
    std::string GetName() const { return name; }

    /// @brief Getter de la descripcion del objeto
    std::string GetDescription() const { return description; }

    /// @brief Getter de las estadisticas del objeto
    Stats GetStats() const { return stats; }

    /// @brief Getter de la rareza del objeto
    ItemRarity GetItemRarity() const { return itemRarity; }

    /// @brief Getter del precio del item
    int GetPrice() const { return price; }

    // Setters
    /// @brief Setter del nombre del objeto
    void SetName(const std::string &newName) { name = newName; }

    /// @brief Setter de la descripcion del objeto
    void SetDescription(const std::string &newDescription) { description = newDescription; }

    /// @brief Setter de las estadisticas del objeto
    void SetStats(const Stats &newStats) { stats = newStats; }

    /// @brief Setter de la rareza del objeto
    void SetItemRarity(ItemRarity newRarity) { itemRarity = newRarity; }

    void SetPrice(int newPrice)
    {
        if (newPrice < 0)
        {
            newPrice = 0;
        }
        price = newPrice;
    }

    virtual ~Item() {}
};
