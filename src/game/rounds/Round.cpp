#include "Round.hpp"
#include <cstdlib>
Round::Round(float duration, float spawnRate, int roundNumber, std::vector<AEnemy*> enemiesToSpawn, std::vector<AEnemy*> enemiesOnMap)
    : duration(duration), spawnRate(spawnRate), roundNumber(roundNumber), enemiesToSpawn(enemiesToSpawn), enemiesOnMap(enemiesOnMap)
{
}

Round::~Round()
{
    for (AEnemy* enemy : enemiesToSpawn)
    {
        delete enemy;
    }
    for (AEnemy* enemy : enemiesOnMap)
    {
        delete enemy;
    }
}

void Round::Update(float deltaTime)
{
    elapsedTime += deltaTime;
    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnRate)
    {
        if (!enemiesToSpawn.empty())
        {
            int randomIndex = rand() % enemiesToSpawn.size();
            AEnemy* enemy = enemiesToSpawn[randomIndex];
            enemiesToSpawn.erase(enemiesToSpawn.begin() + randomIndex);
            enemiesOnMap.push_back(enemy);
        }
        timeSinceLastSpawn = 0.0f;
    }

}

bool Round::IsRoundOver() const
{
    return elapsedTime >= duration || (enemiesOnMap.empty() && enemiesToSpawn.empty());
}