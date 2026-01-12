#include "AudioManager.hpp"
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

AudioManager::~AudioManager()
{
    ClearCache();
}

const Sound &AudioManager::GetSound(WEAPON_TYPE type)
{
    int typeKey = static_cast<int>(type);

    // Verificar si el sonido ya está en caché
    if (weaponSoundsCache.find(typeKey) != weaponSoundsCache.end())
    {
        return weaponSoundsCache[typeKey];
    }

    // Cargar el sonido
    std::string filePath = GetFilePath(type);
    Sound sound = LoadSound(filePath.c_str());

    if (!IsSoundValid(sound))
    {
        throw std::runtime_error("No se pudo cargar el sonido: " + filePath);
    }

    weaponSoundsCache[typeKey] = sound;
    return weaponSoundsCache[typeKey];
}

void AudioManager::PlaySound(WEAPON_TYPE type)
{
    try
    {
        const Sound &sound = GetSound(type);
        int typeKey = static_cast<int>(type);
        
        if (weaponAliasPools.find(typeKey) == weaponAliasPools.end())
        {
            SoundAliasPool pool;
            for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++)
            {
                pool.aliases.push_back(LoadSoundAlias(sound));
            }
            weaponAliasPools[typeKey] = pool;
        }
        
        SoundAliasPool &pool = weaponAliasPools[typeKey];
        int availableIndex = -1;
        
        for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++)
        {
            if (!IsSoundPlaying(pool.aliases[i]))
            {
                availableIndex = i;
                break;
            }
        }
        
        if (availableIndex != -1)
        {
            ::PlaySound(pool.aliases[availableIndex]);
        }
    }
    catch (const std::exception &e)
    {
        TraceLog(LOG_WARNING, "Error reproduciendo sonido de arma: %s", e.what());
    }
}

void AudioManager::ClearCache()
{
    ClearCacheWeapons();
    ClearCacheEnemies();
}

void AudioManager::ClearCacheWeapons()
{
    for (auto &pair : weaponAliasPools)
    {
        for (auto &alias : pair.second.aliases)
        {
            UnloadSoundAlias(alias);
        }
    }
    weaponAliasPools.clear();

    for (auto &pair : weaponSoundsCache)
    {
        UnloadSound(pair.second);
    }
    weaponSoundsCache.clear();
}

void AudioManager::ClearCacheEnemies()
{
    for (auto &pair : enemyAliasPools)
    {
        for (auto &alias : pair.second.aliases)
        {
            UnloadSoundAlias(alias);
        }
    }
    enemyAliasPools.clear();

    for (auto &pair : enemySoundsCache)
    {
        UnloadSound(pair.second);
    }
    enemySoundsCache.clear();
}

void AudioManager::ClearCache(WEAPON_TYPE type)
{
    int typeKey = static_cast<int>(type);

    if (weaponAliasPools.find(typeKey) != weaponAliasPools.end())
    {
        for (auto &alias : weaponAliasPools[typeKey].aliases)
        {
            UnloadSoundAlias(alias);
        }
        weaponAliasPools.erase(typeKey);
    }

    if (weaponSoundsCache.find(typeKey) != weaponSoundsCache.end())
    {
        UnloadSound(weaponSoundsCache[typeKey]);
        weaponSoundsCache.erase(typeKey);
    }
}

void AudioManager::ClearCache(ENEMY_TYPE type)
{
    int typeKey = static_cast<int>(type);

    if (enemyAliasPools.find(typeKey) != enemyAliasPools.end())
    {
        for (auto &alias : enemyAliasPools[typeKey].aliases)
        {
            UnloadSoundAlias(alias);
        }
        enemyAliasPools.erase(typeKey);
    }

    if (enemySoundsCache.find(typeKey) != enemySoundsCache.end())
    {
        UnloadSound(enemySoundsCache[typeKey]);
        enemySoundsCache.erase(typeKey);
    }
}

void AudioManager::SetAssetsRoot(const std::string &assetsRoot)
{
    std::string root = assetsRoot;
    if (!root.empty() && root.back() != '/' && root.back() != '\\')
    {
        root.push_back('/');
    }

    BASE_PATH_WEAPON_SOUNDS = root + "sounds/weapons/";
    BASE_PATH_ENEMY_SOUNDS = root + "sounds/enemies/";
    BASE_PATH_MUSIC = root + "sounds/music/";

    ClearCache();
}

void AudioManager::DetectAndSetAssetsPath()
{
    try
    {
        if (fs::is_directory("assets"))
        {
            SetAssetsRoot("assets");
            return;
        }
        if (fs::is_directory("/usr/share/PablosElMultiverso/assets"))
        {
            SetAssetsRoot("/usr/share/PablosElMultiverso/assets");
            return;
        }
    }
    catch (...)
    {
    }
}

std::string AudioManager::GetFilePath(WEAPON_TYPE type) const
{
    switch (type)
    {
    case WEAPON_TYPE::AXE:
        return BASE_PATH_WEAPON_SOUNDS + "axe.wav";
    case WEAPON_TYPE::SWORD:
        return BASE_PATH_WEAPON_SOUNDS + "sword.wav";
    case WEAPON_TYPE::SCYTHE:
        return BASE_PATH_WEAPON_SOUNDS + "scythe.wav";
    case WEAPON_TYPE::WAND:
        return BASE_PATH_WEAPON_SOUNDS + "wand.wav";
    case WEAPON_TYPE::EGGPLOSIVE:
        return BASE_PATH_WEAPON_SOUNDS + "chicken.wav";
    case WEAPON_TYPE::LASER_RAY:
        return BASE_PATH_WEAPON_SOUNDS + "laser_ray.wav";
    case WEAPON_TYPE::SNIPER:
        return BASE_PATH_WEAPON_SOUNDS + "sniper.wav";
    case WEAPON_TYPE::WING:
        return BASE_PATH_WEAPON_SOUNDS + "wing.wav";
    default:
        throw std::runtime_error("Tipo de arma desconocida");
    }
}

const Sound &AudioManager::GetEnemySound()
{
    int typeKey = 0;

    // Verificar si el sonido ya está en caché
    if (enemySoundsCache.find(typeKey) != enemySoundsCache.end())
    {
        return enemySoundsCache[typeKey];
    }

    // Cargar el sonido
    std::string filePath = GetFilePathEnemy();
    Sound sound = LoadSound(filePath.c_str());

    if (!IsSoundValid(sound))
    {
        throw std::runtime_error("No se pudo cargar el sonido: " + filePath);
    }

    enemySoundsCache[typeKey] = sound;
    return enemySoundsCache[typeKey];
}

void AudioManager::PlayEnemySound()
{
    try
    {
        const Sound &sound = GetEnemySound();
        int typeKey = 0;
        
        if (enemyAliasPools.find(typeKey) == enemyAliasPools.end())
        {
            SoundAliasPool pool;
            for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++)
            {
                pool.aliases.push_back(LoadSoundAlias(sound));
            }
            enemyAliasPools[typeKey] = pool;
        }
        
        SoundAliasPool &pool = enemyAliasPools[typeKey];
        int availableIndex = -1;
        
        for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++)
        {
            if (!IsSoundPlaying(pool.aliases[i]))
            {
                availableIndex = i;
                break;
            }
        }
        
        if (availableIndex != -1)
        {
            ::PlaySound(pool.aliases[availableIndex]);
        }
    }
    catch (const std::exception &e)
    {
        TraceLog(LOG_WARNING, "Error reproduciendo sonido de enemigo: %s", e.what());
    }
}

void AudioManager::PlayBackgroundMusic(const std::string &musicFile)
{
    if (IsMusicValid(currentMusic))
    {
        UnloadMusicStream(currentMusic);
    }

    // Cargar nueva música
    std::string filePath = BASE_PATH_MUSIC + musicFile;
    currentMusic = LoadMusicStream(filePath.c_str());

    if (!IsMusicValid(currentMusic))
    {
        TraceLog(LOG_WARNING, "Error cargando música: %s", filePath.c_str());
        return;
    }

    currentMusic.looping = true;
    currentMusicFile = musicFile;
    ::PlayMusicStream(currentMusic);
    TraceLog(LOG_INFO, "Música reproduciendo: %s", filePath.c_str());
}

void AudioManager::UpdateMusic()
{
    if (!currentMusicFile.empty() && IsMusicValid(currentMusic))
    {
        UpdateMusicStream(currentMusic);
    }
}

std::string AudioManager::GetFilePathEnemy() const
{
    return BASE_PATH_ENEMY_SOUNDS + "enemy.wav";
}
