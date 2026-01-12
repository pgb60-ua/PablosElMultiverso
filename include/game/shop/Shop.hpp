#pragma once

#include "ShopSlot.hpp"
#include <array>

class Shop
{

public:
    constexpr static int MAX_ITEMS_SHOP = 5;
    // Distribución de probabilidad de niveles de armas: Nivel 1 (70%), Nivel 2 (15%), Nivel 3 (10%), Nivel 4 (5%)
    constexpr static int WEAPON_LEVEL_WEIGHTS[4] = {70, 15, 10, 5};

    Shop();
    ~Shop();
    void reRoll();
    const std::array<TShopSlot, MAX_ITEMS_SHOP> &GetItemsShop() const;
    void AlternateBlockSlot(int index);
    bool IsSlotBlocked(int index) const;
    const Item *BuyItem(int index);
    bool IsSlotBuyed(int index) const;
    // Pongo private abajo porque necesito la constante
private:
    std::array<TShopSlot, MAX_ITEMS_SHOP> shopPool;

    /// @brief Genera un nivel aleatorio para un arma según las probabilidades definidas
    /// @param item El item para el cual generar el nivel
    /// @return El nivel del arma (1-4), o 1 si no es un arma
    static int GenerateWeaponLevel(const Item *item);

    /// @brief Valida si un índice está dentro del rango válido
    /// @param index El índice a validar
    /// @return true si el índice es válido, false en caso contrario
    bool IsValidIndex(int index) const;
};
