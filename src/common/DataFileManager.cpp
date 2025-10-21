#include "Types.hpp"
#include <DataFileManager.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <variant>

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
    case ITEM_TYPE::WEAPON:
        return BASE_PATH_ITEM + "weapon.json";
    case ITEM_TYPE::ITEM1:
        return BASE_PATH_ITEM + "item1.json";
    case ITEM_TYPE::ITEM2:
        return BASE_PATH_ITEM + "item2.json";
    case ITEM_TYPE::ITEM3:
        return BASE_PATH_ITEM + "item3.json";
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
    case ENEMY_TYPE::ENEMY2:
        return BASE_PATH_ENEMY + "enemy2.json";
    case ENEMY_TYPE::ENEMY3:
        return BASE_PATH_ENEMY + "enemy3.json";
    case ENEMY_TYPE::ENEMY4:
        return BASE_PATH_ENEMY + "enemy4.json";
    default:
        throw std::runtime_error("Unknown ENEMY type");
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

void DataFileManager::ClearCache()
{
    playerCache.clear();
    itemCache.clear();
    enemyCache.clear();
}

void DataFileManager::ClearCachePlayers() { playerCache.clear(); }

void DataFileManager::ClearCacheItems() { itemCache.clear(); }

void DataFileManager::ClearCacheEnemies() { enemyCache.clear(); }

void DataFileManager::ClearCache(PLAYER_TYPE type) { playerCache.erase(type); }

void DataFileManager::ClearCache(ITEM_TYPE type) { itemCache.erase(type); }

void DataFileManager::ClearCache(ENEMY_TYPE type) { enemyCache.erase(type); }

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

        // Convertir cada elemento del JSON al DataMap
        for (auto &[key, value] : j.items())
        {
            if (value.is_string())
            {
                data[key] = value.get<std::string>();
            }
            else if (value.is_number_integer())
            {
                data[key] = value.get<int>();
            }
            else if (value.is_number_float())
            {
                data[key] = value.get<float>();
            }
            else if (value.is_boolean())
            {
                data[key] = value.get<bool>();
            }
            // Si es otro tipo (objeto, array), se ignora por ahora
        }
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
