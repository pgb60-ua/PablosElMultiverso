#pragma once

#include "Types.hpp"
#include <string>

typedef struct
{
    std::string name;
    int id;
    PLAYER_TYPE type;
} PlayerComponent;
