#include "AWeapon.hpp"
#include "SpriteLoaderManager.hpp"
#include <SpriteSheet.hpp>
#include <cmath>

class AEnemy;

AWeapon::AWeapon(const std::string &name, const std::string &description, const Stats &stats, ItemRarity itemRarity,
                 int level, const Vector2 &position, const std::vector<AEnemy *> &enemiesInRange, const std::vector<AEnemy *> &allEnemies)
    : Item(name, description, stats, itemRarity), level(level), position(position), enemiesInRange(enemiesInRange), allEnemies(allEnemies)
{
}

std::string AWeapon::GetStringFromJSON(const std::string &key, WEAPON_TYPE type, const std::string &defaultValue)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(type);
    auto it = data.find(key);
    if (it != data.end())
    {
        if (const std::string *val = std::get_if<std::string>(&it->second))
        {
            return *val;
        }
    }
    return defaultValue;
}

int AWeapon::GetIntFromJSON(const std::string &key, WEAPON_TYPE type, int defaultValue)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(type);
    auto it = data.find(key);
    if (it != data.end())
    {
        if (const int *val = std::get_if<int>(&it->second))
        {
            return *val;
        }
        if (const float *val = std::get_if<float>(&it->second))
        {
            return static_cast<int>(*val);
        }
    }
    return defaultValue;
}

ItemRarity AWeapon::GetRarityFromJSON(WEAPON_TYPE type)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(type);
    auto it = data.find("rarity");
    if (it != data.end())
    {
        if (const std::string *val = std::get_if<std::string>(&it->second))
        {
            if (*val == "Common")
                return ItemRarity::Common;
            if (*val == "Uncommon")
                return ItemRarity::Uncommon;
            if (*val == "Rare")
                return ItemRarity::Rare;
            if (*val == "Epic")
                return ItemRarity::Epic;
        }
    }
    return ItemRarity::Common;
}

bool AWeapon::Upgrade(const OffensiveStats &newOffensiveStats)
{
    if (level >= MAXLEVEL)
    {
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
        std::min(currentStats.lifeSteal + newOffensiveStats.lifeSteal, MAX_LIFE_STEAL)};

    stats.SetOffensiveStats(upgradedStats);

    return true;
}

void AWeapon::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(static_cast<WEAPON_TYPE>(weaponType));
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width * 0.5f, src.height * 0.5f};

    Rectangle dest = {position.x, position.y, src.width, src.height};

    float angle = atan2(direction.y, direction.x) * RAD2DEG + 45.0f;

    DrawTexturePro(sheet.texture, src, dest, origin, angle, WHITE);
}

Vector2 AWeapon::CalculateDirection()
{
    float closestDistanceSquared = std::numeric_limits<float>::max();
    Vector2 closestToEnemy = {0.0f, 1.0f};

    for (const AEnemy *enemy : enemiesInRange)
    {
        Vector2 toEnemy = {enemy->GetPosition().x - position.x, enemy->GetPosition().y - position.y};
        float distanceSquared = toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y;
        if (distanceSquared > 0.0f && distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            closestToEnemy = toEnemy;
        }
    }

    if (closestDistanceSquared < std::numeric_limits<float>::max())
    {
        float distance = sqrt(closestDistanceSquared);
        if (distance > 0.0f)
        {
            return {closestToEnemy.x / distance, closestToEnemy.y / distance};
        }
    }
    return {0.0f, 1.0f};
}
void AWeapon::update(float deltaTime, const Vector2 &position)
{
    SetPosition(position);
    SetDirection(CalculateDirection());
}