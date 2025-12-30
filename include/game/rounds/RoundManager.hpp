#pragma once
#include "DataFileManager.hpp"
#include "Types.hpp"
#include <vector>
#include <memory>
#include <Round.hpp>



class RoundManager
{
private:
    std::vector<RoundInfo> roundInfos;
    std::vector<AEnemy*> enemiesOnMap;
    std::vector<Player*> players;
    std::vector<AEnemy*> CreateEnemiesForRound(const RoundInfo& roundInfo);
    Round currentRound;
public:
    RoundManager(const std::vector<ROUND_TYPE>& roundTypes, std::vector<AEnemy*> enemiesOnMap, std::vector<Player*> players);
    ~RoundManager() = default;
    void Update(float deltaTime);
    bool IsCurrentRoundOver() const;

    Round GetCurrentRound() const;

    /// @brief Avanza a la siguiente ronda si existe
    /// @return true si se avanzó a la siguiente ronda, false si no hay más rondas
    bool MoveToNextRound();
};