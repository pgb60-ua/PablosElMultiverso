#pragma once
#include "Item.hpp"
#include <string>


enum class WeaponType
{
    Melee,
    Ranged
};

class AWeapon : public Item {
private:
    /// @brief Máxima probabilidad de crítico
    const float MAX_CRITICAL_CHANCE = 30.0f;
    /// @brief Máximo multiplicador de daño crítico
    const float MAX_CRITICAL_DAMAGE = 10.0f;
    /// @brief Máximo porcentaje de robo de vida
    const float MAX_LIFE_STEAL = 50.0f;

protected:
    /// @brief Tipo de arma
    WeaponType weaponType;
    /// @brief Nivel del arma
    int level;
    /// @brief Nivel máximo del arma
    const int MAXLEVEL = 4;

public:
    /// @brief Constructor de la clase Weapon
    AWeapon(const std::string& name, const std::string& description, const Stats& stats, ItemRarity itemRarity, WeaponType weaponType, int level);
    /// @brief Getter del tipo de arma
    WeaponType GetWeaponType() const { return weaponType; }
    /// @brief Getter del nivel del arma
    int GetLevel() const { return level; }
    /// @brief Getter del nivel máximo del arma
    int GetMaxLevel() const { return MAXLEVEL; }
    /// @brief Sube de nivel el arma
    bool Upgrade(const OffensiveStats& newOffensiveStats);


    /// @brief Método para atacar
    virtual void Attack() = 0;
    virtual ~AWeapon() {}

};