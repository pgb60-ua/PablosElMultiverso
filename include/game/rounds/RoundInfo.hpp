#pragma once
#include "Types.hpp"
#include <unordered_map>

struct RoundInfo
{
    int roundNumber;
    float duration;
    float spawnRate;
    std::unordered_map<ENEMY_TYPE, int> enemiesToSpawnCount;
};
