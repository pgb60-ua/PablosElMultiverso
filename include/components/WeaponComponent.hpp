#pragma once

#include <EnTT/entt.hpp>

typedef struct
{
    entt::entity playerOwnerEntity;
    float offsetX;
    float offsetY;
    entt::entity targetEnemy; // Enemigo más cercano al que apuntar
} WeaponComponent;
