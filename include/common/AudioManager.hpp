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
    std::string BASE_PATH_PROJECTILE_SOUNDS = "assets/sounds/projectiles/";
    std::string BASE_PATH_WEAPON_SOUNDS = "assets/sounds/weapons/";
    std::string BASE_PATH_ENEMY_SOUNDS = "assets/sounds/enemies/";
    std::string BASE_PATH_MUSIC = "assets/sounds/music/";

    static AudioManager &GetInstance()
    {
        static AudioManager instance;
        return instance;
    }

    /// @brief Obtiene el sonido para un tipo de proyectil
    const Sound &GetSound(PROJECTILE_TYPE type);

    /// @brief Reproduce un sonido por tipo de proyectil
    void PlaySound(PROJECTILE_TYPE type);

    /// @brief Obtiene el sonido para un tipo de arma
    const Sound &GetSound(WEAPON_TYPE type);

    /// @brief Reproduce un sonido por tipo de arma
    void PlaySound(WEAPON_TYPE type);

    /// @brief Obtiene el sonido para un tipo de enemigo
    const Sound &GetEnemySound(ENEMY_TYPE type);

    /// @brief Reproduce un sonido por tipo de enemigo
    void PlayEnemySound(ENEMY_TYPE type);

    /// @brief Carga y reproduce música de fondo en loop
    void PlayBackgroundMusic(const std::string &musicFile);

    /// @brief Detiene la música de fondo
    void StopBackgroundMusic();

    /// @brief Pausa la música de fondo
    void PauseBackgroundMusic();

    /// @brief Reanuda la música de fondo
    void ResumeBackgroundMusic();

    /// @brief Establece el volumen de la música (0.0 a 1.0)
    void SetMusicVolume(float volume);

    /// @brief Obtiene el volumen actual de la música
    float GetMusicVolume() const;

    /// @brief Verifica si hay música reproduciéndose
    bool IsMusicPlaying() const;

    // ========== CACHÉ Y LIMPIEZA ==========
    /// @brief Limpia toda la caché de sonidos
    void ClearCache();

    /// @brief Limpia la caché de sonidos de proyectiles
    void ClearCacheProjectiles();

    /// @brief Limpia la caché de sonidos de armas
    void ClearCacheWeapons();

    /// @brief Limpia la caché de sonidos de enemigos
    void ClearCacheEnemies();

    /// @brief Limpia la caché de un sonido de proyectil específico
    void ClearCache(PROJECTILE_TYPE type);

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

    std::string GetFilePath(PROJECTILE_TYPE type) const;
    std::string GetFilePath(WEAPON_TYPE type) const;
    std::string GetFilePath(ENEMY_TYPE type) const;

    std::unordered_map<int, Sound> projectileSoundsCache;
    std::unordered_map<int, Sound> weaponSoundsCache;
    std::unordered_map<int, Sound> enemySoundsCache;
    
    static constexpr int MAX_CONCURRENT_SOUNDS = 4;
    struct SoundAliasPool {
        std::vector<Sound> aliases;
    };
    
    std::unordered_map<int, SoundAliasPool> projectileAliasPools;
    std::unordered_map<int, SoundAliasPool> weaponAliasPools;
    std::unordered_map<int, SoundAliasPool> enemyAliasPools;
    
    Music currentMusic = {};
    std::string currentMusicFile = "";
    float currentMusicVolume = 1.0f;

    std::string assetsRoot = "";
};
