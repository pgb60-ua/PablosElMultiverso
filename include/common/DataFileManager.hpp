#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include "Types.hpp"


// Tipo para los valores que puede contener el diccionario
using DataValue = std::variant<std::string, int, float, bool>;
// Tipo para el diccionario de datos
using DataMap = std::unordered_map<std::string, DataValue>;

class DataFileManager
{
    const std::string BASE_PATH_PLAYER = "assets/data/players/";
    const std::string BASE_PATH_ITEM = "assets/data/items/";

public:
    static DataFileManager &GetInstance()
    {
        static DataFileManager instance;
        return instance;
    }

    const DataMap &GetData(PLAYER_TYPE type);
    const DataMap &GetData(ITEM_TYPE type);

    void ClearCache();
    void ClearCachePlayers();
    void ClearCacheItems();
    void ClearCache(PLAYER_TYPE type);
    void ClearCache(ITEM_TYPE type);

private:
    DataFileManager() = default;
    ~DataFileManager() = default;

    DataFileManager(const DataFileManager &) = delete;
    DataFileManager &operator=(const DataFileManager &) = delete;
    DataFileManager(DataFileManager &&) = delete;
    DataFileManager &operator=(DataFileManager &&) = delete;

    std::string GetFilePath(PLAYER_TYPE type) const;
    std::string GetFilePath(ITEM_TYPE type) const;

    DataMap LoadFromFile(const std::string &path);

    std::unordered_map<PLAYER_TYPE, DataMap> playerCache;
    std::unordered_map<ITEM_TYPE, DataMap> itemCache;
};