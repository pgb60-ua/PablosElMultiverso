#include <DataFileManager.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>
#include <variant>

std::string DataFileManager::GetFilePath(PLAYER type) const
{
    switch (type)
    {
    case PLAYER::WARRIOR:
        return BASE_PATH_PLAYER + "warrior.json";
    case PLAYER::MAGE:
        return BASE_PATH_PLAYER + "mage.json";
    case PLAYER::RANGE:
        return BASE_PATH_PLAYER + "range.json";
    case PLAYER::HEALER:
        return BASE_PATH_PLAYER + "healer.json";
    default:
        throw std::runtime_error("Unknown PLAYER type");
    }
}

std::string DataFileManager::GetFilePath(ITEM type) const
{
    switch (type)
    {
    case ITEM::WEAPON:
        return BASE_PATH_ITEM + "weapon.json";
    case ITEM::ITEM1:
        return BASE_PATH_ITEM + "item1.json";
    case ITEM::ITEM2:
        return BASE_PATH_ITEM + "item2.json";
    case ITEM::ITEM3:
        return BASE_PATH_ITEM + "item3.json";
    default:
        throw std::runtime_error("Unknown ITEM type");
    }
}

const DataMap &DataFileManager::GetData(PLAYER type)
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

const DataMap &DataFileManager::GetData(ITEM type)
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

void DataFileManager::ClearCache()
{
    playerCache.clear();
    itemCache.clear();
}

void DataFileManager::ClearCachePlayers() { playerCache.clear(); }

void DataFileManager::ClearCacheItems() { itemCache.clear(); }

void DataFileManager::ClearCache(PLAYER type) { playerCache.erase(type); }

void DataFileManager::ClearCache(ITEM type) { itemCache.erase(type); }

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

Stats DataFileManager::GetPlayerStats(PLAYER type)
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
