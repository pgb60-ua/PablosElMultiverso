#pragma once
#include "stats.hpp"
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
    /// @brief Textura del item
    Texture2D sprite;

public:
    /// @brief Constructor de la clase Item

    Item(std::string name, std::string description, Stats stats, ItemRarity itemRarity, Texture2D sprite);

    /// @brief Getter del nombre del objeto
    std::string GetName() const { return name; }

    /// @brief Getter de la descripcion del objeto
    std::string GetDescription() const { return description; }

    /// @brief Getter de las estadisticas del objeto
    Stats GetStats() const { return stats; }

    /// @brief Getter de la rareza del objeto
    ItemRarity GetItemRarity() const { return itemRarity; }

    // Setters
    /// @brief Setter del nombre del objeto
    void SetName(const std::string &newName) { name = newName; }

    /// @brief Setter de la descripcion del objeto
    void SetDescription(const std::string &newDescription) { description = newDescription; }

    /// @brief Setter de las estadisticas del objeto
    void SetStats(const Stats &newStats) { stats = newStats; }

    /// @brief Setter de la rareza del objeto
    void SetItemRarity(ItemRarity newRarity) { itemRarity = newRarity; }

    virtual ~Item() {}
};
