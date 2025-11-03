#pragma once
#include "Stats.hpp"
#include "Types.hpp"
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// Tipo para los valores que puede contener el diccionario
using DataValue = std::variant<std::string, int, float, bool>;
// Tipo para el diccionario de datos
using DataMap = std::unordered_map<std::string, DataValue>;

class DataFileManager
{
    const std::string BASE_PATH_PLAYER = "assets/data/players/";
    const std::string BASE_PATH_ITEM = "assets/data/items/";
    const std::string BASE_PATH_ENEMY = "assets/data/enemies/";
    const std::string BASE_PATH_WEAPON = "assets/data/weapons/";

public:
    static DataFileManager &GetInstance()
    {
        static DataFileManager instance;
        return instance;
    }

    const DataMap &GetData(PLAYER_TYPE type);
    const DataMap &GetData(ITEM_TYPE type);
    const DataMap &GetData(ENEMY_TYPE type);
    const DataMap &GetData(WEAPON_TYPE type);

    void ClearCache();
    void ClearCachePlayers();
    void ClearCacheItems();
    void ClearCacheEnemies();
    void ClearCacheWeapons();
    void ClearCache(PLAYER_TYPE type);
    void ClearCache(ITEM_TYPE type);
    void ClearCache(ENEMY_TYPE type);
    void ClearCache(WEAPON_TYPE type);

    Stats GetPlayerStats(PLAYER_TYPE type);
    Stats GetEnemyStats(ENEMY_TYPE type);
    Stats GetWeaponStats(WEAPON_TYPE type);

private:
    DataFileManager() = default;
    ~DataFileManager() = default;

    DataFileManager(const DataFileManager &) = delete;
    DataFileManager &operator=(const DataFileManager &) = delete;
    DataFileManager(DataFileManager &&) = delete;
    DataFileManager &operator=(DataFileManager &&) = delete;

    std::string GetFilePath(PLAYER_TYPE type) const;
    std::string GetFilePath(ITEM_TYPE type) const;
    std::string GetFilePath(ENEMY_TYPE type) const;
    std::string GetFilePath(WEAPON_TYPE type) const;

    DataMap LoadFromFile(const std::string &path);

    std::unordered_map<PLAYER_TYPE, DataMap> playerCache;
    std::unordered_map<ITEM_TYPE, DataMap> itemCache;
    std::unordered_map<ENEMY_TYPE, DataMap> enemyCache;
    std::unordered_map<WEAPON_TYPE, DataMap> weaponCache;
};
