#include <Weapon.hpp>

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
        std::min(currentStats.criticalChance + newOffensiveStats.criticalChance, 30.0f),
        std::min(currentStats.criticalDamage + newOffensiveStats.criticalDamage, 10.0f),
        std::min(currentStats.lifeSteal + newOffensiveStats.lifeSteal, 50.0f)
    };

    stats.SetOffensiveStats(upgradedStats);

    return true;
}