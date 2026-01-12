#pragma once
#include "DataFileManager.hpp"
#include "Types.hpp"
#include "Round.hpp"
#include <vector>
#include <memory>




class RoundManager
{
private:
    std::vector<RoundInfo> roundInfos;
    std::vector<AEnemy*>& enemiesOnMap;
    std::vector<Player*>& players;
    std::vector<AEnemy*> CreateEnemiesForRound(const RoundInfo& roundInfo);
    void ClearEnemiesFromMap();
    Round currentRound;
public:
    RoundManager(const ROUND_TYPE roundType, std::vector<AEnemy*>& enemiesOnMap, std::vector<Player*>& players);
    ~RoundManager() = default;
    void Update(float deltaTime);
    void Render();
    bool IsCurrentRoundOver() const;

    const Round& GetCurrentRound() const;

    /// @brief Avanza a la siguiente ronda si existe
    /// @return true si se avanzó a la siguiente ronda, false si no hay más rondas
    bool MoveToNextRound();
};