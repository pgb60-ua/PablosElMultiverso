#pragma once
#include <string>
#include "stats.hpp"
extern "C" {
    #include <raylib.h>
}

enum class ItemRarity
{
    Common,
    Uncommon,
    Rare,
    Epic
};

class Item {
    protected:
        /// @brief nombre del objeto
        std::string name;
        /// @brief  Descripción del objeto
        std::string description;
        /// @brief Estadisticas del objeto
        Stats stats;
        /// @brief Rareza del objeto
        ItemRarity itemRarity;

    public:
        /// @brief Constructor de la clase Item

        Item(std::string name, std::string description, Stats stats, ItemRarity itemRarity);

        /// @brief Getter del nombre del objeto
        std::string getName() const { return name; }

        /// @brief Getter de la descripcion del objeto
        std::string getDescription() const { return description; }

        /// @brief Getter de las estadisticas del objeto
        Stats getStats() const { return stats; }

        /// @brief Getter de la rareza del objeto
        ItemRarity getItemRarity() const { return itemRarity; }

        // Setters
        /// @brief Setter del nombre del objeto
        void setName(const std::string& newName) { name = newName; }

        /// @brief Setter de la descripcion del objeto
        void setDescription(const std::string& newDescription) { description = newDescription; }

        /// @brief Setter de las estadisticas del objeto
        void setStats(const Stats& newStats) { stats = newStats; }

        /// @brief Setter de la rareza del objeto
        void setItemRarity(ItemRarity newRarity) { itemRarity = newRarity; }

        virtual ~Item() {}
};