#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

enum class PLAYER {
    WARRIOR,
    MAGE,
    RANGE,
    HEALER
};

enum class ITEM {
    ITEM1,
    ITEM2,
    ITEM3,
    WEAPON
};

// Tipo para los valores que puede contener el diccionario
using DataValue = std::variant<std::string, int, float, bool>;
// Tipo para el diccionario de datos
using DataMap = std::unordered_map<std::string, DataValue>;

class DataFileManager {
public:
    static DataFileManager& getInstance() {
        static DataFileManager instance;
        return instance;
    }
    
    // Obtener datos como diccionario (con caché)
    const DataMap& GetData(PLAYER type);
    const DataMap& GetData(ITEM type);
    
    // Limpiar caché
    void clearCache();
    void clearCache(PLAYER type);
    void clearCache(ITEM type);

private:
    DataFileManager() = default;
    ~DataFileManager() = default;
    
    DataFileManager(const DataFileManager&) = delete;
    DataFileManager& operator=(const DataFileManager&) = delete;
    DataFileManager(DataFileManager&&) = delete;
    DataFileManager& operator=(DataFileManager&&) = delete;

    // Métodos auxiliares
    std::string getFilePath(PLAYER type) const;
    std::string getFilePath(ITEM type) const;
    
    // Cargar y parsear archivo a diccionario
    DataMap loadFromFile(const std::string& path);
    
    // Cachés para cada tipo de enum
    std::unordered_map<PLAYER, DataMap> playerCache;
    std::unordered_map<ITEM, DataMap> itemCache;
};  