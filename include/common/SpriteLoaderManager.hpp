#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <raylib.h>
#include "Types.hpp"
#include "FrameAnimation.hpp"
#include "Geometry.hpp"
#include "SpriteSheet.hpp"

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
    const SpriteSheet &GetSpriteSheet(PLAYER_TYPE type);
    // Obtiene el sprite-sheet para un tipo de item
    const SpriteSheet &GetSpriteSheet(ITEM_TYPE type);
    // Obtiene el sprite-sheet para un tipo de enemigo
    const SpriteSheet &GetSpriteSheet(ENEMY_TYPE type);
    // Obtiene el sprite-sheet para un tipo de proyectil
    const SpriteSheet &GetSpriteSheet(PROJECTILE_TYPE type);
    // Obtiene el sprite-sheet para un tipo de arma
    const SpriteSheet &GetSpriteSheet(WEAPON_TYPE type);
    // Obtiene el sprite-sheet para un tipo de mapa
    const SpriteSheet &GetSpriteSheet(MAP_TYPE type);

    const Shape& GetSpriteHitbox(PLAYER_TYPE type, Vector2 position);

    // Limpia toda la caché
    void ClearCache();

    // Limpia la caché de jugadores
    void ClearCachePlayers();
    // Limpia la caché de items
    void ClearCacheItems();
    // Limpia la caché de enemigos
    void ClearCacheEnemies();
    // Limpia la caché de proyectiles
    void ClearCacheProjectiles();
    // Limpia la caché de armas
    void ClearCacheWeapons();
    // Limpia la caché de mapas
    void ClearCacheMaps();

    // Limpia la caché de un jugador específico
    void ClearCache(PLAYER_TYPE type);
    // Limpia la caché de un item específico
    void ClearCache(ITEM_TYPE type);
    // Limpia la caché de un enemigo específico
    void ClearCache(ENEMY_TYPE type);
    // Limpia la caché de un proyectil específico
    void ClearCache(PROJECTILE_TYPE type);
    // Limpia la caché de un arma específica
    void ClearCache(WEAPON_TYPE type);
    // Limpia la caché de un mapa específico
    void ClearCache(MAP_TYPE type);

private:
    SpriteLoaderManager() = default;
    ~SpriteLoaderManager();

    SpriteLoaderManager(const SpriteLoaderManager &) = delete;
    SpriteLoaderManager &operator=(const SpriteLoaderManager &) = delete;
    SpriteLoaderManager(SpriteLoaderManager &&) = delete;
    SpriteLoaderManager &operator=(SpriteLoaderManager &&) = delete;

    // Obtiene la ruta del archivo de metadatos para un tipo de jugador
    std::string GetMetadataPath(PLAYER_TYPE type) const;
    // Obtiene la ruta del archivo de metadatos para un tipo de item
    std::string GetMetadataPath(ITEM_TYPE type) const;
    // Obtiene la ruta del archivo de metadatos para un tipo de enemigo
    std::string GetMetadataPath(ENEMY_TYPE type) const;
    // Obtiene la ruta del archivo de metadatos para un tipo de proyectil
    std::string GetMetadataPath(PROJECTILE_TYPE type) const;
    // Obtiene la ruta del archivo de metadatos para un tipo de arma
    std::string GetMetadataPath(WEAPON_TYPE type) const;
    // Obtiene la ruta del archivo de metadatos para un tipo de mapa
    std::string GetMetadataPath(MAP_TYPE type) const;

    // Carga un sprite-sheet desde un archivo de metadatos JSON
    SpriteSheet LoadSpriteSheetFromMetadata(const std::string &metadataPath);

    // Libera un sprite-sheet (textura y frames)
    void UnloadSpriteSheet(SpriteSheet &spriteSheet);

    // Caché de sprite-sheets por tipo
    std::unordered_map<PLAYER_TYPE, SpriteSheet> playerSpriteCache;
    std::unordered_map<ITEM_TYPE, SpriteSheet> itemSpriteCache;
    std::unordered_map<ENEMY_TYPE, SpriteSheet> enemySpriteCache;
    std::unordered_map<PROJECTILE_TYPE, SpriteSheet> projectileSpriteCache;
    std::unordered_map<WEAPON_TYPE, SpriteSheet> weaponSpriteCache;
    std::unordered_map<MAP_TYPE, SpriteSheet> mapSpriteCache;
};