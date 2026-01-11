#pragma once

#include "Item.hpp"

typedef struct
{
    const Item *item;
    bool isBlocked;
    bool isBuyed;
    int weaponLevel; // Nivel del arma si es un arma (1-4), 1 por defecto
} TShopSlot;
