#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include "Types.hpp"

// Estructura para almacenar un sprite-sheet con sus frames
struct SpriteSheet
{
    Texture2D texture;              // La textura completa del sprite-sheet
    std::vector<Rectangle> frames;  // Rectángulos de cada frame en el sprite-sheet
};

class SpriteLoaderManager
{
    const std::string BASE_PATH_PLAYER = "assets/sprites/players/";
    const std::string BASE_PATH_ITEM = "assets/sprites/items/";

public:
    static SpriteLoaderManager &GetInstance()
    {
        static SpriteLoaderManager instance;
        return instance;
    }

    // Obtiene el sprite-sheet para un tipo de jugador
    const SpriteSheet &GetSpriteSheet(PLAYER type);
    
    // Obtiene el sprite-sheet para un tipo de item
    const SpriteSheet &GetSpriteSheet(ITEM type);
    
    // Limpia toda la caché
    void ClearCache();
    
    // Limpia la caché de jugadores
    void ClearCachePlayers();
    
    // Limpia la caché de items
    void ClearCacheItems();
    
    // Limpia la caché de un jugador específico
    void ClearCache(PLAYER type);
    
    // Limpia la caché de un item específico
    void ClearCache(ITEM type);

private:
    SpriteLoaderManager() = default;
    ~SpriteLoaderManager();

    SpriteLoaderManager(const SpriteLoaderManager &) = delete;
    SpriteLoaderManager &operator=(const SpriteLoaderManager &) = delete;
    SpriteLoaderManager(SpriteLoaderManager &&) = delete;
    SpriteLoaderManager &operator=(SpriteLoaderManager &&) = delete;

    // Obtiene la ruta del archivo de metadatos para un tipo de jugador
    std::string GetMetadataPath(PLAYER type) const;
    
    // Obtiene la ruta del archivo de metadatos para un tipo de item
    std::string GetMetadataPath(ITEM type) const;

    // Carga un sprite-sheet desde un archivo de metadatos JSON
    SpriteSheet LoadSpriteSheetFromMetadata(const std::string &metadataPath);
    
    // Libera un sprite-sheet (textura y frames)
    void UnloadSpriteSheet(SpriteSheet &spriteSheet);

    // Caché de sprite-sheets por tipo
    std::unordered_map<PLAYER, SpriteSheet> playerSpriteCache;
    std::unordered_map<ITEM, SpriteSheet> itemSpriteCache;
};