#pragma once
#include "DataFileManager.hpp"
#include "Types.hpp"
#include "AEnemy.hpp"
#include "RoundInfo.hpp"
#include <vector>
#include <unordered_map>

class Round
{
private:
    std::vector<AEnemy*> enemiesToSpawn;
    std::vector<AEnemy*> enemiesOnMap;
    int roundNumber;
    float duration;
    float spawnRate;
    float timeSinceLastSpawn = 0.0f;
    float elapsedTime = 0.0f;

public:
    Round(float duration, float spawnRate,int roundNumber, std::vector<AEnemy*> enemiesToSpawn, std::vector<AEnemy*> enemiesOnMap);
    ~Round();

    void Update(float deltaTime);
    bool IsRoundOver() const;

    int GetRoundNumber() const { return roundNumber; }
};