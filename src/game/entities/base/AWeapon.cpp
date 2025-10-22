#include "AWeapon.hpp"
#include "SpriteLoaderManager.hpp"
#include <SpriteSheet.hpp>
#include <cmath>

AWeapon::AWeapon(const std::string &name, const std::string &description, const Stats &stats, ItemRarity itemRarity,
                 int level, const Vector2 &position)
    : Item(name, description, stats, itemRarity), level(level), position(position)
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
            if (*val == "Common") return ItemRarity::Common;
            if (*val == "Uncommon") return ItemRarity::Uncommon;
            if (*val == "Rare") return ItemRarity::Rare;
            if (*val == "Epic") return ItemRarity::Epic;
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
        std::min(currentStats.lifeSteal + newOffensiveStats.lifeSteal, MAX_LIFE_STEAL)
    };

    stats.SetOffensiveStats(upgradedStats);

    return true;
}

void AWeapon::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(static_cast<WEAPON_TYPE>(weaponType));
    if (sheet.frames.empty()) return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = { src.width * 0.5f, src.height * 0.5f };

    Rectangle dest = { position.x, position.y, src.width, src.height };

    float angle = atan2(direction.y, direction.x) * RAD2DEG + 45.0f;

    DrawTexturePro(sheet.texture, src, dest, origin, angle, WHITE);
}

Vector2 AWeapon::CalculateDirection() {
    Vector2 dir = {0.0f, 1.0f};
    
    return dir;
}
void AWeapon::update(float deltaTime, const Vector2& position) {
    SetPosition(position);
    SetDirection(CalculateDirection());
}