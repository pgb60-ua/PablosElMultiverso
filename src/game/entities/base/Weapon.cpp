#include <Weapon.hpp>

const float MAX_CRITICAL_CHANCE = 30.0f;
const float MAX_CRITICAL_DAMAGE = 10.0f;
const float MAX_LIFE_STEAL = 50.0f;

Weapon::Weapon(const std::string& name, const std::string& description, const Stats& stats, ItemRarity itemRarity, WeaponType weaponType, int level)
    : Item(name, description, stats, itemRarity), weaponType(weaponType), level(level) {}


bool Weapon::Upgrade(const OffensiveStats& newOffensiveStats) {
    if (level >= maxLevel) {
        return false;
    }
    level++;

    OffensiveStats currentStats = stats.GetOffensiveStats();
    OffensiveStats upgradedStats = {
        currentStats.physicalDamage + newOffensiveStats.physicalDamage,
        currentStats.magicDamage + newOffensiveStats.magicDamage,
        currentStats.attackSpeed + newOffensiveStats.attackSpeed,
        std::min(currentStats.criticalChance + newOffensiveStats.criticalChance, MAX_CRITICAL_CHANCE),
        std::min(currentStats.criticalDamage + newOffensiveStats.criticalDamage, MAX_CRITICAL_DAMAGE),
        std::min(currentStats.lifeSteal + newOffensiveStats.lifeSteal, MAX_LIFE_STEAL)
    };

    stats.SetOffensiveStats(upgradedStats);

    return true;
}