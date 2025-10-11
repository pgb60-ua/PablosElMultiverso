#include <DataFileManager.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

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

void DataFileManager::ClearCache()
{
    playerCache.clear();
    itemCache.clear();
}

void DataFileManager::ClearCachePlayers()
{
    playerCache.clear();
}

void DataFileManager::ClearCacheItems()
{
    itemCache.clear();
}

void DataFileManager::ClearCache(PLAYER_TYPE type)
{
    playerCache.erase(type);
}

void DataFileManager::ClearCache(ITEM_TYPE type)
{
    itemCache.erase(type);
}

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
