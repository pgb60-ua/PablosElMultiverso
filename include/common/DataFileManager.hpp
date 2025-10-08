#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

enum class PLAYER
{
    WARRIOR,
    MAGE,
    RANGE,
    HEALER
};

enum class ITEM
{
    ITEM1,
    ITEM2,
    ITEM3,
    WEAPON
};

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

    // Obtener datos como diccionario (con caché)
    const DataMap &GetData(PLAYER type);
    const DataMap &GetData(ITEM type);

    // Limpiar caché
    void ClearCache();
    void ClearCache(PLAYER type);
    void ClearCache(ITEM type);

private:
    DataFileManager() = default;
    ~DataFileManager() = default;

    DataFileManager(const DataFileManager &) = delete;
    DataFileManager &operator=(const DataFileManager &) = delete;
    DataFileManager(DataFileManager &&) = delete;
    DataFileManager &operator=(DataFileManager &&) = delete;

    // Métodos auxiliares
    std::string GetFilePath(PLAYER type) const;
    std::string GetFilePath(ITEM type) const;

    // Cargar y parsear archivo a diccionario
    DataMap LoadFromFile(const std::string &path);

    // Cachés para cada tipo de enum
    std::unordered_map<PLAYER, DataMap> playerCache;
    std::unordered_map<ITEM, DataMap> itemCache;
};