#include <SpriteLoaderManager.hpp>
#include <stdexcept>
#include <fstream>
#include <nlohmann/json.hpp>

SpriteLoaderManager::~SpriteLoaderManager()
{   
    ClearCache();
}   

std::string SpriteLoaderManager::GetMetadataPath(PLAYER type) const
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

std::string SpriteLoaderManager::GetMetadataPath(ITEM type) const
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

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(PLAYER type)
{
    // Buscar en caché
    auto it = playerSpriteCache.find(type);
    if (it != playerSpriteCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo de metadatos
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);

    // Guardar en caché y devolver referencia
    playerSpriteCache[type] = std::move(spriteSheet);
    return playerSpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(ITEM type)
{
    // Buscar en caché
    auto it = itemSpriteCache.find(type);
    if (it != itemSpriteCache.end())
    {
        return it->second;
    }

    // No está en caché, cargar desde archivo de metadatos
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);

    // Guardar en caché y devolver referencia
    itemSpriteCache[type] = std::move(spriteSheet);
    return itemSpriteCache[type];
}

void SpriteLoaderManager::ClearCache()
{
    ClearCachePlayers();
    ClearCacheItems();
}

void SpriteLoaderManager::ClearCachePlayers()
{
    for (auto &[type, spriteSheet] : playerSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    playerSpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheItems()
{
    for (auto &[type, spriteSheet] : itemSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    itemSpriteCache.clear();
}

void SpriteLoaderManager::ClearCache(PLAYER type)
{
    auto it = playerSpriteCache.find(type);
    if (it != playerSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        playerSpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(ITEM type)
{
    auto it = itemSpriteCache.find(type);
    if (it != itemSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        itemSpriteCache.erase(it);
    }
}

SpriteSheet SpriteLoaderManager::LoadSpriteSheetFromMetadata(const std::string &metadataPath)
{
    SpriteSheet spriteSheet;
    
    // Abrir archivo de metadatos
    std::ifstream file(metadataPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open metadata file: " + metadataPath);
    }

    try
    {
        // Parsear el archivo JSON
        nlohmann::json j;
        file >> j;

        // Obtener la ruta de la textura (relativa al archivo de metadatos)
        if (!j.contains("texture"))
        {
            throw std::runtime_error("Missing 'texture' field in metadata file: " + metadataPath);
        }

        std::string texturePath = j["texture"].get<std::string>();
        
        // Si la ruta es relativa, construir la ruta completa
        // Obtener el directorio del archivo de metadatos
        size_t lastSlash = metadataPath.find_last_of("/\\");
        std::string baseDir = (lastSlash != std::string::npos) 
            ? metadataPath.substr(0, lastSlash + 1) 
            : "";
        
        std::string fullTexturePath = baseDir + texturePath;

        // Cargar la textura
        spriteSheet.texture = LoadTexture(fullTexturePath.c_str());
        
        if (spriteSheet.texture.id == 0)
        {
            throw std::runtime_error("Failed to load texture: " + fullTexturePath);
        }

        // Leer los frames
        if (!j.contains("frames"))
        {
            throw std::runtime_error("Missing 'frames' field in metadata file: " + metadataPath);
        }

        const auto &framesJson = j["frames"];
        if (!framesJson.is_array())
        {
            throw std::runtime_error("'frames' must be an array in metadata file: " + metadataPath);
        }

        // Parsear cada frame
        for (const auto &frameJson : framesJson)
        {
            if (!frameJson.contains("x") || !frameJson.contains("y") || 
                !frameJson.contains("width") || !frameJson.contains("height"))
            {
                // Limpiar y lanzar error
                UnloadTexture(spriteSheet.texture);
                throw std::runtime_error("Frame missing required fields (x, y, width, height) in: " + metadataPath);
            }

            Rectangle frame = {
                frameJson["x"].get<float>(),
                frameJson["y"].get<float>(),
                frameJson["width"].get<float>(),
                frameJson["height"].get<float>()
            };

            spriteSheet.frames.push_back(frame);
        }

        if (spriteSheet.frames.empty())
        {
            UnloadTexture(spriteSheet.texture);
            throw std::runtime_error("No frames defined in metadata file: " + metadataPath);
        }
    }
    catch (const nlohmann::json::exception &e)
    {
        if (spriteSheet.texture.id != 0)
        {
            UnloadTexture(spriteSheet.texture);
        }
        throw std::runtime_error("Error parsing JSON metadata file " + metadataPath + ": " + e.what());
    }

    return spriteSheet;
}

void SpriteLoaderManager::UnloadSpriteSheet(SpriteSheet &spriteSheet)
{
    UnloadTexture(spriteSheet.texture);
    spriteSheet.frames.clear();
}
