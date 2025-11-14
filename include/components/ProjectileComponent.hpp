#pragma once

#include "Types.hpp"
#include <EnTT/entt.hpp>

typedef struct
{
    entt::entity weaponOwner;
    PROJECTILE_TYPE type;
    bool active;
} ProjectileComponent;
