#include "DataFileManager.hpp"
#include "Item.hpp"
#include "RoundInfo.hpp"
#include "Types.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include <stdexcept>
#include <variant>

void DataFileManager::SetAssetsRoot(const std::string &assetsRoot)
{
    std::string root = assetsRoot;
    if (!root.empty() && root.back() != '/' && root.back() != '\\')
    {
        root.push_back('/');
    }

    BASE_PATH_PLAYER = root + "data/players/";
    BASE_PATH_ITEM = root + "data/items/";
    BASE_PATH_ENEMY = root + "data/enemies/";
    BASE_PATH_WEAPON = root + "data/weapons/";
    BASE_PATH_ROUND = root + "data/rounds/";
}

void DataFileManager::DetectAndSetAssetsPath()
{
    namespace fs = std::filesystem;
    try
    {
        if (fs::is_directory("assets"))
        {
            SetAssetsRoot("assets");
            return;
        }
        if (fs::is_directory("/usr/share/PablosElMultiverso/assets"))
        {
            SetAssetsRoot("/usr/share/PablosElMultiverso/assets");
            return;
        }
    }
    catch (...)
    {
    }
}

std::string DataFileManager::GetFilePath(PLAYER_TYPE type) const
{
    switch (type)
    {
    case PLAYER_TYPE::WARRIOR:
        return BASE_PATH_PLAYER + "warrior.json";
    case PLAYER_TYPE::MAGE:
        return BASE_PATH_PLAYER + "mage.json";
    case PLAYER_TYPE::RANGE:
        return BASE_PATH_PLAYER + "range.json";
    case PLAYER_TYPE::HEALER:
        return BASE_PATH_PLAYER + "healer.json";
    default:
        throw std::runtime_error("Unknown PLAYER type");
    }
}

std::string DataFileManager::GetFilePath(ITEM_TYPE type) const
{
    switch (type)
    {
    case ITEM_TYPE::FIREBALL:
        return BASE_PATH_ITEM + "Fireball.json";
    case ITEM_TYPE::SPINED_BREASTPLATE:
        return BASE_PATH_ITEM + "SpinedBreastplate.json";
    case ITEM_TYPE::PEARL_EGG:
        return BASE_PATH_ITEM + "PearlEgg.json";
    case ITEM_TYPE::RAVENS_FEATHER:
        return BASE_PATH_ITEM + "Raven'sFeather.json";
    case ITEM_TYPE::VOID_RING:
        return BASE_PATH_ITEM + "VoidRing.json";
    case ITEM_TYPE::SATCHEL:
        return BASE_PATH_ITEM + "Satchel.json";
    case ITEM_TYPE::SPIRIT_MASK:
        return BASE_PATH_ITEM + "SpiritMask.json";
    case ITEM_TYPE::CELESTIAL_SPARK:
        return BASE_PATH_ITEM + "CelestialSpark.json";
    case ITEM_TYPE::EXPLOSIVE_ARSENAL:
        return BASE_PATH_ITEM + "ExplosiveArsenal.json";
    case ITEM_TYPE::MAGICAL_MIRROR:
        return BASE_PATH_ITEM + "MagicalMirror.json";
    case ITEM_TYPE::ETHEREAL_DAGGER:
        return BASE_PATH_ITEM + "EtherealDagger.json";
    case ITEM_TYPE::SPOTLIGHT:
        return BASE_PATH_ITEM + "Spotlight.json";
    case ITEM_TYPE::CRIMSON_VIAL:
        return BASE_PATH_ITEM + "CrimsonVial.json";
    case ITEM_TYPE::RUBY_HEART:
        return BASE_PATH_ITEM + "RubyHeart.json";
    case ITEM_TYPE::RAW_MEAT:
        return BASE_PATH_ITEM + "RawMeat.json";
    case ITEM_TYPE::VENOMOUS_PLANT:
        return BASE_PATH_ITEM + "VenomousPlant.json";
    case ITEM_TYPE::WEAPON_AXE:
        return BASE_PATH_WEAPON + "axe.json";
    case ITEM_TYPE::WEAPON_SWORD:
        return BASE_PATH_WEAPON + "sword.json";
    case ITEM_TYPE::WEAPON_SCYTHE:
        return BASE_PATH_WEAPON + "scythe.json";
    case ITEM_TYPE::WEAPON_WAND:
        return BASE_PATH_WEAPON + "wand.json";
    case ITEM_TYPE::WEAPON_EGGPLOSIVE:
        return BASE_PATH_WEAPON + "eggplosive.json";
    case ITEM_TYPE::WEAPON_LASER_RAY:
        return BASE_PATH_WEAPON + "laser_ray.json";
    case ITEM_TYPE::WEAPON_SNIPER:
        return BASE_PATH_WEAPON + "sniper.json";
    case ITEM_TYPE::WEAPON_WING:
        return BASE_PATH_WEAPON + "wing.json";
    case ITEM_TYPE::COIN:
        throw std::runtime_error("COIN is a sprite-only type and has no data file");
    default:
        throw std::runtime_error("Unknown ITEM type");
    }
}

std::string DataFileManager::GetFilePath(ENEMY_TYPE type) const
{
    switch (type)
    {
    case ENEMY_TYPE::ZOMBIE:
        return BASE_PATH_ENEMY + "zombie.json";
    case ENEMY_TYPE::DARKIN:
        return BASE_PATH_ENEMY + "darkin.json";
    case ENEMY_TYPE::CHEMICAL_DESTRUCTOR:
        return BASE_PATH_ENEMY + "chemical_destructor.json";
    default:
        throw std::runtime_error("Unknown ENEMY type");
    }
}

std::string DataFileManager::GetFilePath(WEAPON_TYPE type) const
{
    switch (type)
    {
    case WEAPON_TYPE::AXE:
        return BASE_PATH_WEAPON + "axe.json";
    case WEAPON_TYPE::SWORD:
        return BASE_PATH_WEAPON + "sword.json";
    case WEAPON_TYPE::SCYTHE:
        return BASE_PATH_WEAPON + "scythe.json";
    case WEAPON_TYPE::WAND:
        return BASE_PATH_WEAPON + "wand.json";
    case WEAPON_TYPE::EGGPLOSIVE:
        return BASE_PATH_WEAPON + "eggplosive.json";
    case WEAPON_TYPE::LASER_RAY:
        return BASE_PATH_WEAPON + "laser_ray.json";
    case WEAPON_TYPE::SNIPER:
        return BASE_PATH_WEAPON + "sniper.json";
    case WEAPON_TYPE::WING:
        return BASE_PATH_WEAPON + "wing.json";
    case WEAPON_TYPE::CHEMICAL_DESTRUCTOR:
        return BASE_PATH_WEAPON + "chemical_destructor.json";
    default:
        throw std::runtime_error("Unknown WEAPON type");
    }
}

std::string DataFileManager::GetFilePath(ROUND_TYPE type) const
{
    switch (type)
    {
    case ROUND_TYPE::EASY:
        return BASE_PATH_ROUND + "easy.json";
    case ROUND_TYPE::MEDIUM:
        return BASE_PATH_ROUND + "medium.json";
    case ROUND_TYPE::HARD:
        return BASE_PATH_ROUND + "hard.json";
    default:
        throw std::runtime_error("Unknown ROUND type");
    }
}

const DataMap &DataFileManager::GetData(PLAYER_TYPE type)
{
    // Buscar en caché
    auto it = playerCache.find(type);
    if (it != playerCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo
    std::string path = GetFilePath(type);
    DataMap data = LoadFromFile(path);

    // Guardar en caché y devolver referencia
    playerCache[type] = std::move(data);
    return playerCache[type];
}

const DataMap &DataFileManager::GetData(ITEM_TYPE type)
{
    // Buscar en caché
    auto it = itemCache.find(type);
    if (it != itemCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo
    std::string path = GetFilePath(type);
    DataMap data = LoadFromFile(path);

    // Guardar en caché y devolver referencia
    itemCache[type] = std::move(data);
    return itemCache[type];
}

const DataMap &DataFileManager::GetData(ENEMY_TYPE type)
{
    // Buscar en caché
    auto it = enemyCache.find(type);
    if (it != enemyCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo
    std::string path = GetFilePath(type);
    DataMap data = LoadFromFile(path);

    // Guardar en caché y devolver referencia
    enemyCache[type] = std::move(data);
    return enemyCache[type];
}

const DataMap &DataFileManager::GetData(WEAPON_TYPE type)
{
    // Buscar en caché
    auto it = weaponCache.find(type);
    if (it != weaponCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo
    std::string path = GetFilePath(type);
    DataMap data = LoadFromFile(path);

    // Guardar en caché y devolver referencia
    weaponCache[type] = std::move(data);
    return weaponCache[type];
}

const DataMap &DataFileManager::GetData(ROUND_TYPE type)
{
    // Buscar en caché
    auto it = roundCache.find(type);
    if (it != roundCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo
    std::string path = GetFilePath(type);
    DataMap data = LoadFromFile(path);

    // Guardar en caché y devolver referencia
    roundCache[type] = std::move(data);
    return roundCache[type];
}

void DataFileManager::ClearCache()
{
    playerCache.clear();
    itemCache.clear();
    enemyCache.clear();
}

void DataFileManager::ClearCachePlayers() { playerCache.clear(); }

void DataFileManager::ClearCacheItems() { itemCache.clear(); }

void DataFileManager::ClearCacheEnemies() { enemyCache.clear(); }

void DataFileManager::ClearCacheWeapons() { weaponCache.clear(); }

void DataFileManager::ClearCacheRounds() { roundCache.clear(); }

void DataFileManager::ClearCache(PLAYER_TYPE type) { playerCache.erase(type); }

void DataFileManager::ClearCache(ITEM_TYPE type) { itemCache.erase(type); }

void DataFileManager::ClearCache(ENEMY_TYPE type) { enemyCache.erase(type); }

void DataFileManager::ClearCache(WEAPON_TYPE type) { weaponCache.erase(type); }

void DataFileManager::ClearCache(ROUND_TYPE type) { roundCache.erase(type); }

// Cargar y parsear archivo a diccionario
DataMap DataFileManager::LoadFromFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + path);
    }

    DataMap data;

    try
    {
        // Parsear el archivo JSON
        nlohmann::json j;
        file >> j;

        // Función helper recursiva para aplanar JSON
        std::function<void(const nlohmann::json &, const std::string &)> flattenJson;
        flattenJson = [&data, &flattenJson](const nlohmann::json &jsonObj, const std::string &prefix)
        {
            if (jsonObj.is_object())
            {
                for (const auto &[key, value] : jsonObj.items())
                {
                    std::string newKey = prefix.empty() ? key : prefix + key;

                    if (value.is_string())
                    {
                        data[newKey] = value.get<std::string>();
                    }
                    else if (value.is_number_integer())
                    {
                        data[newKey] = value.get<int>();
                    }
                    else if (value.is_number_float())
                    {
                        data[newKey] = value.get<float>();
                    }
                    else if (value.is_boolean())
                    {
                        data[newKey] = value.get<bool>();
                    }
                    else if (value.is_array())
                    {
                        // Guardar el tamaño del array
                        data[newKey + "_count"] = static_cast<int>(value.size());

                        // Procesar cada elemento del array
                        for (size_t i = 0; i < value.size(); ++i)
                        {
                            flattenJson(value[i], newKey + "_" + std::to_string(i) + "_");
                        }
                    }
                    else if (value.is_object())
                    {
                        // Procesar objetos anidados
                        flattenJson(value, newKey + "_");
                    }
                }
            }
            else if (jsonObj.is_array())
            {
                // Si el root es un array
                data[prefix + "count"] = static_cast<int>(jsonObj.size());
                for (size_t i = 0; i < jsonObj.size(); ++i)
                {
                    flattenJson(jsonObj[i], prefix + std::to_string(i) + "_");
                }
            }
        };

        // Aplanar el JSON
        flattenJson(j, "");
    }
    catch (const nlohmann::json::exception &e)
    {
        throw std::runtime_error("Error parsing JSON file " + path + ": " + e.what());
    }

    return data;
}

// ============================================================================
// Métodos de conveniencia para obtener datos específicos de jugadores
// ============================================================================

Stats DataFileManager::GetPlayerStats(PLAYER_TYPE type)
{
    const DataMap &data = GetData(type);

    // Helper lambda optimizada - usa get_if que es más rápido
    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            // std::get_if es más eficiente que holds_alternative + get
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    // Crear OffensiveStats desde el JSON (todas las keys coinciden con el JSON)
    OffensiveStats offensiveStats = {
        getFloat("physical_damage"),         // physicalDamage
        getFloat("magical_damage"),          // magicDamage
        getFloat("attack_speed", 1.0f),      // attackSpeed
        getFloat("critical_chance"),         // criticalChance
        getFloat("critical_damage", 100.0f), // criticalDamage
        getFloat("life_steal")               // lifeSteal
    };

    // Crear DefensiveStats desde el JSON (todas las keys coinciden con el JSON)
    DefensiveStats defensiveStats = {
        getFloat("health", 100.0f),       // health
        getFloat("max_health", 100.0f),   // healthMax
        getFloat("movement_speed", 5.0f), // movementSpeed
        getFloat("agility"),              // agility
        getFloat("armor"),                // armor
        getFloat("resistance"),           // resistance
        getFloat("health_regeneration")   // healthRegeneration
    };

    return Stats(offensiveStats, defensiveStats);
}

ItemData DataFileManager::GetItemData(ITEM_TYPE type)
{
    const DataMap &data = GetData(type);

    auto getString = [&data](const std::string &key, const std::string &defaultValue = "") -> std::string
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            if (const std::string *val = std::get_if<std::string>(&it->second))
            {
                return *val;
            }
        }
        return defaultValue;
    };

    auto getInt = [&data](const std::string &key, int defaultValue = 0) -> int
    {
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
    };

    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    // Parsear datos básicos
    std::string name = getString("name", "Unknown Item");
    std::string description = getString("description", "");
    int price = getInt("price", 0);

    // Parsear rareza
    std::string rarityStr = getString("rarity", "Common");
    ItemRarity rarity = ItemRarity::Common;
    if (rarityStr == "Uncommon")
        rarity = ItemRarity::Uncommon;
    else if (rarityStr == "Rare")
        rarity = ItemRarity::Rare;
    else if (rarityStr == "Epic")
        rarity = ItemRarity::Epic;

    // Parsear stats
    OffensiveStats offensiveStats = {getFloat("physical_damage"),       getFloat("magical_damage"),
                                     getFloat("attack_speed", 0.0f),    getFloat("critical_chance"),
                                     getFloat("critical_damage", 0.0f), getFloat("life_steal")};

    DefensiveStats defensiveStats = {
        getFloat("health", 0.0f), getFloat("max_health", 0.0f), getFloat("movement_speed", 0.0f), getFloat("agility"),
        getFloat("armor"),        getFloat("resistance"),       getFloat("health_regeneration")};

    Stats stats(offensiveStats, defensiveStats);

    return ItemData{name, description, stats, rarity, price};
}

Stats DataFileManager::GetEnemyStats(ENEMY_TYPE type)
{
    const DataMap &data = GetData(type);

    // Helper lambda optimizada - usa get_if que es más rápido
    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            // std::get_if es más eficiente que holds_alternative + get
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    // Crear OffensiveStats desde el JSON (todas las keys coinciden con el JSON)
    OffensiveStats offensiveStats = {
        getFloat("physical_damage"),         // physicalDamage
        getFloat("magical_damage"),          // magicDamage
        getFloat("attack_speed", 1.0f),      // attackSpeed
        getFloat("critical_chance"),         // criticalChance
        getFloat("critical_damage", 100.0f), // criticalDamage
        getFloat("life_steal")               // lifeSteal
    };

    // Crear DefensiveStats desde el JSON (todas las keys coinciden con el JSON)
    DefensiveStats defensiveStats = {
        getFloat("health", 100.0f),       // health
        getFloat("max_health", 100.0f),   // healthMax
        getFloat("movement_speed", 5.0f), // movementSpeed
        getFloat("agility"),              // agility
        getFloat("armor"),                // armor
        getFloat("resistance"),           // resistance
        getFloat("health_regeneration")   // healthRegeneration
    };

    return Stats(offensiveStats, defensiveStats);
}

// ============================================================================
// Métodos de conveniencia para obtener datos específicos de armas
// ============================================================================

Stats DataFileManager::GetWeaponStats(WEAPON_TYPE type)
{
    const DataMap &data = GetData(type);

    // Helper lambda optimizada - usa get_if que es más rápido
    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            // std::get_if es más eficiente que holds_alternative + get
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    // Crear OffensiveStats desde el JSON (todas las keys coinciden con el JSON)
    OffensiveStats offensiveStats = {
        getFloat("physical_damage"),         // physicalDamage
        getFloat("magical_damage"),          // magicDamage
        getFloat("attack_speed", 1.0f),      // attackSpeed
        getFloat("critical_chance"),         // criticalChance
        getFloat("critical_damage", 100.0f), // criticalDamage
        getFloat("life_steal")               // lifeSteal
    };

    // Crear DefensiveStats desde el JSON (todas las keys coinciden con el JSON)
    DefensiveStats defensiveStats = {
        getFloat("health", 100.0f),       // health
        getFloat("max_health", 100.0f),   // healthMax
        getFloat("movement_speed", 5.0f), // movementSpeed
        getFloat("agility"),              // agility
        getFloat("armor"),                // armor
        getFloat("resistance"),           // resistance
        getFloat("health_regeneration")   // healthRegeneration
    };

    return Stats(offensiveStats, defensiveStats);
}

// ============================================================================
// Métodos para obtener datos específicos de rondas
// ============================================================================

std::vector<RoundInfo> DataFileManager::GetRounds(ROUND_TYPE type)
{
    // Obtener el DataMap cacheado
    const DataMap &data = GetData(type);
    std::vector<RoundInfo> rounds;

    // Helper lambdas para extraer datos del DataMap
    auto getInt = [&data](const std::string &key, int defaultValue = 0) -> int
    {
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
    };

    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    // Parsear las rondas del DataMap
    int roundsCount = getInt("rounds_count", 0);

    for (int i = 0; i < roundsCount; ++i)
    {
        std::string prefix = "rounds_" + std::to_string(i) + "_";

        RoundInfo roundInfo;
        roundInfo.roundNumber = getInt(prefix + "roundNumber", 0);
        roundInfo.duration = getFloat(prefix + "duration", 60.0f);
        roundInfo.spawnRate = getFloat(prefix + "spawnRate", 5.0f);

        // Parsear enemigos - el objeto enemiesToSpawn se aplana como enemiesToSpawn_KEY
        // Necesitamos iterar manualmente sobre las claves del DataMap que coincidan
        for (const auto &[key, value] : data)
        {
            std::string enemyPrefix = prefix + "enemiesToSpawn_";
            if (key.find(enemyPrefix) == 0 && key.find("_count") == std::string::npos)
            {

                std::string enemyName = key.substr(enemyPrefix.length());
                int enemyCount = getInt(key, 0);

                if (!enemyName.empty() && enemyCount > 0)
                {
                    ENEMY_TYPE enemyType;

                    // Convertir string a ENEMY_TYPE
                    if (enemyName == "ZOMBIE")
                        enemyType = ENEMY_TYPE::ZOMBIE;
                    else if (enemyName == "DARKIN")
                        enemyType = ENEMY_TYPE::DARKIN;
                    else if (enemyName == "CHEMICAL_DESTRUCTOR")
                        enemyType = ENEMY_TYPE::CHEMICAL_DESTRUCTOR;
                    else
                    {
                        spdlog::warn("DataFileManager::GetRounds: Unknown ENEMY_TYPE '{}'", enemyName);
                        continue; // Ignorar tipos desconocidos
                    }
                    roundInfo.enemiesToSpawnCount[enemyType] = enemyCount;
                }
            }
        }

        rounds.push_back(roundInfo);
    }

    return rounds;
}

// ============================================================================
// Métodos de conveniencia para obtener datos específicos de items
// ============================================================================

Stats DataFileManager::GetItemStats(ITEM_TYPE type)
{
    const DataMap &data = GetData(type);

    auto getFloat = [&data](const std::string &key, float defaultValue = 0.0f) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return defaultValue;
    };

    OffensiveStats offensiveStats = {
        getFloat("physical_damage"),       // physicalDamage
        getFloat("magical_damage"),        // magicDamage
        getFloat("attack_speed", 0.0f),    // attackSpeed
        getFloat("critical_chance"),       // criticalChance
        getFloat("critical_damage", 0.0f), // criticalDamage
        getFloat("life_steal")             // lifeSteal
    };

    DefensiveStats defensiveStats = {
        getFloat("health", 0.0f),         // health
        getFloat("max_health", 0.0f),     // healthMax
        getFloat("movement_speed", 0.0f), // movementSpeed
        getFloat("agility"),              // agility
        getFloat("armor"),                // armor
        getFloat("resistance"),           // resistance
        getFloat("health_regeneration")   // healthRegeneration
    };

    return Stats(offensiveStats, defensiveStats);
}
