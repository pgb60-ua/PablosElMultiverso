#include <Stats.hpp>

Stats::Stats()
{
}

Stats::Stats(const OffensiveStats &offensiveStats, const DefensiveStats &defensiveStats)
    : offensiveStats(offensiveStats), defensiveStats(defensiveStats) {};

void Stats::SetOffensiveStats(const OffensiveStats &offensiveStats) { this->offensiveStats = offensiveStats; }

void Stats::SetDefensiveStats(const DefensiveStats &defensiveStats) { this->defensiveStats = defensiveStats; }
