#include "RoundManager.hpp"
#include "Zombie.hpp"
#include "Darkin.hpp"
#include <functional>
#include <new>

RoundManager::RoundManager(const ROUND_TYPE roundType, std::vector<AEnemy*>& enemiesOnMap, std::vector<Player*>& players)
    : enemiesOnMap(enemiesOnMap), players(players), currentRound(0.0f, 0.0f, 0, std::vector<AEnemy*>(), enemiesOnMap)
{
    DataFileManager& dataManager = DataFileManager::GetInstance();
    this->roundInfos = dataManager.GetRounds(roundType);
}

std::vector<AEnemy*> RoundManager::CreateEnemiesForRound(const RoundInfo& roundInfo)
{
    std::vector<AEnemy*> enemies;
    int totalEnemies = 0;
    for (const auto& [_, count] : roundInfo.enemiesToSpawnCount)
        totalEnemies += count;

    enemies.reserve(totalEnemies);

    std::unordered_map<ENEMY_TYPE, std::function<AEnemy*()>> enemyFactories =
    {
        {ENEMY_TYPE::ZOMBIE,[&]() -> AEnemy*{return new Zombie(players);}},
        {ENEMY_TYPE::DARKIN,[&]() -> AEnemy*{return new Darkin(players);}},
        //{ENEMY_TYPE::CHEMICAL_DESTRUCTOR,[&]() -> AEnemy*{return new ChemicalDestructor(players);}}

    };

    for (const auto& [enemyType, count] : roundInfo.enemiesToSpawnCount)
    {
        auto it = enemyFactories.find(enemyType);
        if (it == enemyFactories.end()){
            continue;
        }

        for (int i = 0; i < count; ++i)
        {
            enemies.push_back(it->second());
        }
    }

    return enemies;
}


void RoundManager::Update(float deltaTime)
{
    currentRound.Update(deltaTime);
}

bool RoundManager::IsCurrentRoundOver() const
{
    return currentRound.IsRoundOver();
}

const Round& RoundManager::GetCurrentRound() const
{
    return currentRound;
}

bool RoundManager::MoveToNextRound()
{

    int nextRoundNumber = currentRound.GetRoundNumber() + 1;
    auto it = std::find_if(roundInfos.begin(), roundInfos.end(),
    [nextRoundNumber](const RoundInfo& info) {
        return info.roundNumber == nextRoundNumber;
    });

    if (it != roundInfos.end())
    {
        RoundInfo& roundInfo = *it;
        std::vector<AEnemy*> enemiesToSpawn = CreateEnemiesForRound(roundInfo);
        currentRound.~Round();
        new (&currentRound) Round(roundInfo.duration, roundInfo.spawnRate, roundInfo.roundNumber, enemiesToSpawn, enemiesOnMap);

        return true;
    }
    return false;
}

void RoundManager::Render()
{
    this->currentRound.Render();
}
