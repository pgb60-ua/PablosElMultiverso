#include <SpriteLoaderManager.hpp>
#include <stdexcept>
#include <fstream>
#include <nlohmann/json.hpp>

SpriteLoaderManager::~SpriteLoaderManager()
{   
    ClearCache();
}   

std::string SpriteLoaderManager::GetMetadataPath(PLAYER_TYPE type) const
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

std::string SpriteLoaderManager::GetMetadataPath(ITEM_TYPE type) const
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

std::string SpriteLoaderManager::GetMetadataPath(ENEMY_TYPE type) const
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

std::string SpriteLoaderManager::GetMetadataPath(PROJECTILE_TYPE type) const
{
    switch (type)
    {
    case PROJECTILE_TYPE::WAND:
        return BASE_PATH_PROJECTILE + "wand.json";
    case PROJECTILE_TYPE::EGGPLOSIVE:
        return BASE_PATH_PROJECTILE + "eggplosive.json";
    case PROJECTILE_TYPE::LASER_RAY:
        return BASE_PATH_PROJECTILE + "laser_ray.json";
    case PROJECTILE_TYPE::SNIPER:
        return BASE_PATH_PROJECTILE + "sniper.json";
    case PROJECTILE_TYPE::WING:
        return BASE_PATH_PROJECTILE + "wing.json";
    default:
        throw std::runtime_error("Unknown PROJECTILE type");
    }
}

std::string SpriteLoaderManager::GetMetadataPath(WEAPON_TYPE type) const
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
    default:
        throw std::runtime_error("Unknown WEAPON type");
    }
}

std::string SpriteLoaderManager::GetMetadataPath(MAP_TYPE type) const
{
    switch (type)
    {
    case MAP_TYPE::DEFAULT:
        return BASE_PATH_MAP + "default.json";
    default:
        throw std::runtime_error("Unknown MAP type");
    }
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(PLAYER_TYPE type)
{
    auto it = playerSpriteCache.find(type);
    if (it != playerSpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    playerSpriteCache[type] = std::move(spriteSheet);
    return playerSpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(ITEM_TYPE type)
{
    auto it = itemSpriteCache.find(type);
    if (it != itemSpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    itemSpriteCache[type] = std::move(spriteSheet);
    return itemSpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(ENEMY_TYPE type)
{
    auto it = enemySpriteCache.find(type);
    if (it != enemySpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    enemySpriteCache[type] = std::move(spriteSheet);
    return enemySpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(PROJECTILE_TYPE type)
{
    auto it = projectileSpriteCache.find(type);
    if (it != projectileSpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    projectileSpriteCache[type] = std::move(spriteSheet);
    return projectileSpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(WEAPON_TYPE type)
{
    auto it = weaponSpriteCache.find(type);
    if (it != weaponSpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    weaponSpriteCache[type] = std::move(spriteSheet);
    return weaponSpriteCache[type];
}

const SpriteSheet &SpriteLoaderManager::GetSpriteSheet(MAP_TYPE type)
{
    auto it = mapSpriteCache.find(type);
    if (it != mapSpriteCache.end())
        return it->second;
    std::string metadataPath = GetMetadataPath(type);
    SpriteSheet spriteSheet = LoadSpriteSheetFromMetadata(metadataPath);
    mapSpriteCache[type] = std::move(spriteSheet);
    return mapSpriteCache[type];
}

Shape SpriteLoaderManager::GetSpriteHitbox(PLAYER_TYPE type, Vector2 position)
{
    const SpriteSheet &spriteSheet = GetSpriteSheet(type);
    const Rectangle &frame = spriteSheet.frames[0];
    Shape hitbox;
    hitbox.type = SHAPE_RECTANGLE;
    hitbox.data.rectangle = { position.x, position.y, frame.width, frame.height };
    return hitbox;
}
void SpriteLoaderManager::ClearCache()
{
    ClearCachePlayers();
    ClearCacheItems();
    ClearCacheEnemies();
    ClearCacheProjectiles();
    ClearCacheWeapons();
    ClearCacheMaps();
}

void SpriteLoaderManager::ClearCachePlayers()
{
    for (auto &[type, spriteSheet] : playerSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    playerSpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheEnemies()
{
    for (auto &[type, spriteSheet] : enemySpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    enemySpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheProjectiles()
{
    for (auto &[type, spriteSheet] : projectileSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    projectileSpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheWeapons()
{
    for (auto &[type, spriteSheet] : weaponSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    weaponSpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheMaps()
{
    for (auto &[type, spriteSheet] : mapSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    mapSpriteCache.clear();
}

void SpriteLoaderManager::ClearCacheItems()
{
    for (auto &[type, spriteSheet] : itemSpriteCache)
    {
        UnloadSpriteSheet(spriteSheet);
    }
    itemSpriteCache.clear();
}

void SpriteLoaderManager::ClearCache(PLAYER_TYPE type)
{
    auto it = playerSpriteCache.find(type);
    if (it != playerSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        playerSpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(ENEMY_TYPE type)
{
    auto it = enemySpriteCache.find(type);
    if (it != enemySpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        enemySpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(PROJECTILE_TYPE type)
{
    auto it = projectileSpriteCache.find(type);
    if (it != projectileSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        projectileSpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(WEAPON_TYPE type)
{
    auto it = weaponSpriteCache.find(type);
    if (it != weaponSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        weaponSpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(MAP_TYPE type)
{
    auto it = mapSpriteCache.find(type);
    if (it != mapSpriteCache.end())
    {
        UnloadSpriteSheet(it->second);
        mapSpriteCache.erase(it);
    }
}

void SpriteLoaderManager::ClearCache(ITEM_TYPE type)
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
