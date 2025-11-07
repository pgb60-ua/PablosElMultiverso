#pragma once

#include "Types.hpp"
#include <string>

typedef struct
{
    std::string name;
    bool dead;
    PLAYER_TYPE type;
} PlayerComponent;
