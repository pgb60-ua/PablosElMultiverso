#pragma once
#include <string>
#include <unordered_map>
#include "Types.hpp"
#include <vector>
extern "C"
{
    #include <raylib.h>
}

class AudioManager
{
public:
    std::string BASE_PATH_SOUNDS = "assets/sounds/";
    std::string BASE_PATH_WEAPON_SOUNDS = "assets/sounds/weapons/";
    std::string BASE_PATH_ENEMY_SOUNDS = "assets/sounds/enemies/";
    std::string BASE_PATH_MUSIC = "assets/sounds/music/";

    static AudioManager &GetInstance()
    {
        static AudioManager instance;
        return instance;
    }

    /// @brief Obtiene el sonido para un tipo de arma
    const Sound &GetSound(WEAPON_TYPE type);

    /// @brief Reproduce un sonido por tipo de arma
    void PlaySound(WEAPON_TYPE type);

    /// @brief Obtiene el sonido para un tipo de enemigo
    const Sound &GetEnemySound();

    /// @brief Reproduce un sonido por tipo de enemigo
    void PlayEnemySound();

    /// @brief Carga y reproduce música de fondo en loop
    void PlayBackgroundMusic(const std::string &musicFile);

    /// @brief Actualiza el stream de música (debe llamarse cada frame)
    void UpdateMusic();

    // ========== CACHÉ Y LIMPIEZA ==========
    /// @brief Limpia toda la caché de sonidos
    void ClearCache();

    /// @brief Limpia la caché de sonidos de armas
    void ClearCacheWeapons();

    /// @brief Limpia la caché de sonidos de enemigos
    void ClearCacheEnemies();

    /// @brief Limpia la caché de un sonido de arma específica
    void ClearCache(WEAPON_TYPE type);

    /// @brief Limpia la caché de un sonido de enemigo específico
    void ClearCache(ENEMY_TYPE type);

    /// @brief Establece la ruta raíz de los assets
    void SetAssetsRoot(const std::string &assetsRoot);

    /// @brief Detecta automáticamente la ruta de assets
    void DetectAndSetAssetsPath();

private:
    AudioManager() = default;
    ~AudioManager();

    AudioManager(const AudioManager &) = delete;
    AudioManager &operator=(const AudioManager &) = delete;
    AudioManager(AudioManager &&) = delete;
    AudioManager &operator=(AudioManager &&) = delete;

    std::string GetFilePath(WEAPON_TYPE type) const;
    std::string GetFilePathEnemy() const;

    std::unordered_map<int, Sound> weaponSoundsCache;
    std::unordered_map<int, Sound> enemySoundsCache;
    
    static constexpr int MAX_CONCURRENT_SOUNDS = 6;
    struct SoundAliasPool {
        std::vector<Sound> aliases;
    };
    
    std::unordered_map<int, SoundAliasPool> weaponAliasPools;
    std::unordered_map<int, SoundAliasPool> enemyAliasPools;
    
    Music currentMusic = {};
    std::string currentMusicFile = "";

    std::string assetsRoot = "";
};
