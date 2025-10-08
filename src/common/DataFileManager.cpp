#include "common/DataFileManager.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Implementación de getFilePath para PLAYER
std::string DataFileManager::getFilePath(PLAYER type) const {
    switch (type) {
        case PLAYER::WARRIOR:
            return "assets/data/player.json";
        case PLAYER::MAGE:
            return "assets/data/mage.json";
        case PLAYER::RANGE:
            return "assets/data/range.json";
        case PLAYER::HEALER:
            return "assets/data/healer.json";
        default:
            throw std::runtime_error("Unknown PLAYER type");
    }
}

// Implementación de getFilePath para ITEM
std::string DataFileManager::getFilePath(ITEM type) const {
    switch (type) {
        case ITEM::WEAPON:
            return "assets/data/items/weapon.json";
        case ITEM::ITEM1:
            return "assets/data/items/item1.json";
        case ITEM::ITEM2:
            return "assets/data/items/item2.json";
        case ITEM::ITEM3:
            return "assets/data/items/item3.json";
        default:
            throw std::runtime_error("Unknown ITEM type");
    }
}

// GetData para PLAYER con caché
const DataMap& DataFileManager::GetData(PLAYER type) {
    // Buscar en caché
    auto it = playerCache.find(type);
    if (it != playerCache.end()) {
        // Ya está en caché, devolverlo
        return it->second;
    }
    
    // No está en caché, cargar desde archivo
    std::string path = getFilePath(type);
    DataMap data = loadFromFile(path);
    
    // Guardar en caché y devolver referencia
    playerCache[type] = std::move(data);
    return playerCache[type];
}

// GetData para ITEM con caché
const DataMap& DataFileManager::GetData(ITEM type) {
    // Buscar en caché
    auto it = itemCache.find(type);
    if (it != itemCache.end()) {
        // Ya está en caché, devolverlo
        return it->second;
    }
    
    // No está en caché, cargar desde archivo
    std::string path = getFilePath(type);
    DataMap data = loadFromFile(path);
    
    // Guardar en caché y devolver referencia
    itemCache[type] = std::move(data);
    return itemCache[type];
}

// Métodos para limpiar caché
void DataFileManager::clearCache() {
    playerCache.clear();
    itemCache.clear();
}

void DataFileManager::clearCache(PLAYER type) {
    playerCache.erase(type);
}

void DataFileManager::clearCache(ITEM type) {
    itemCache.erase(type);
}

// Cargar y parsear archivo a diccionario
DataMap DataFileManager::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
    }
    
    DataMap data;
    
    try {
        // Parsear el archivo JSON
        json j;
        file >> j;
        
        // Convertir cada elemento del JSON al DataMap
        for (auto& [key, value] : j.items()) {
            if (value.is_string()) {
                data[key] = value.get<std::string>();
            } else if (value.is_number_integer()) {
                data[key] = value.get<int>();
            } else if (value.is_number_float()) {
                data[key] = value.get<float>();
            } else if (value.is_boolean()) {
                data[key] = value.get<bool>();
            }
            // Si es otro tipo (objeto, array), se ignora por ahora
        }
    } catch (const json::exception& e) {
        throw std::runtime_error("Error parsing JSON file " + path + ": " + e.what());
    }
    
    return data;
}




