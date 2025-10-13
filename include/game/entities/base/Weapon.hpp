#pragma once
#include <Item.hpp>
#include <string>


enum class WeaponType
{
    Melee,
    Ranged
};

class Weapon : public Item {
protected:
    /// @brief Tipo de arma
    WeaponType weaponType;
    /// @brief Nivel del arma
    int level;
    /// @brief Nivel máximo del arma
    const int maxLevel = 4;

public:
    /// @brief Constructor de la clase Weapon
    Weapon(const std::string& name, const std::string& description, const Stats& stats, ItemRarity itemRarity, WeaponType weaponType, int level);
    /// @brief Getter del tipo de arma
    WeaponType GetWeaponType() const { return weaponType; }
    /// @brief Getter del nivel del arma
    int GetLevel() const { return level; }
    /// @brief Getter del nivel máximo del arma
    int GetMaxLevel() const { return maxLevel; }
    /// @brief Sube de nivel el arma
    virtual bool Upgrade(const OffensiveStats& newOffensiveStats);


    /// @brief Método para atacar
    virtual void Attack() = 0;
    virtual ~Weapon() override = default;

};