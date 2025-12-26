#include "AudioManager.hpp"
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

AudioManager::~AudioManager()
{
    ClearCache();
}

const Sound &AudioManager::GetSound(PROJECTILE_TYPE type)
{
    int typeKey = static_cast<int>(type);

    // Verificar si el sonido ya está en caché
    if (projectileSoundsCache.find(typeKey) != projectileSoundsCache.end())
    {
        return projectileSoundsCache[typeKey];
    }

    // Cargar el sonido
    std::string filePath = GetFilePath(type);
    Sound sound = LoadSound(filePath.c_str());

    if (!IsSoundValid(sound))
    {
        throw std::runtime_error("No se pudo cargar el sonido: " + filePath);
    }

    projectileSoundsCache[typeKey] = sound;
    return projectileSoundsCache[typeKey];
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

void AudioManager::PlaySound(PROJECTILE_TYPE type)
{
    try
    {
        const Sound &sound = GetSound(type);
        int typeKey = static_cast<int>(type);
        
        if (projectileAliasPools.find(typeKey) == projectileAliasPools.end())
        {
            SoundAliasPool pool;
            for (int i = 0; i < MAX_CONCURRENT_SOUNDS; i++)
            {
                pool.aliases.push_back(LoadSoundAlias(sound));
            }
            projectileAliasPools[typeKey] = pool;
        }
        
        SoundAliasPool &pool = projectileAliasPools[typeKey];
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
        TraceLog(LOG_WARNING, "Error reproduciendo sonido de proyectil: %s", e.what());
    }
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
    ClearCacheProjectiles();
    ClearCacheWeapons();
    ClearCacheEnemies();
}

void AudioManager::ClearCacheProjectiles()
{
    for (auto &pair : projectileSoundsCache)
    {
        UnloadSound(pair.second);
    }
    projectileSoundsCache.clear();
}

void AudioManager::ClearCacheWeapons()
{
    for (auto &pair : weaponSoundsCache)
    {
        UnloadSound(pair.second);
    }
    weaponSoundsCache.clear();
}

void AudioManager::ClearCacheEnemies()
{
    for (auto &pair : enemySoundsCache)
    {
        UnloadSound(pair.second);
    }
    enemySoundsCache.clear();
}

void AudioManager::ClearCache(PROJECTILE_TYPE type)
{
    int typeKey = static_cast<int>(type);
    if (projectileSoundsCache.find(typeKey) != projectileSoundsCache.end())
    {
        UnloadSound(projectileSoundsCache[typeKey]);
        projectileSoundsCache.erase(typeKey);
    }
}

void AudioManager::ClearCache(WEAPON_TYPE type)
{
    int typeKey = static_cast<int>(type);
    if (weaponSoundsCache.find(typeKey) != weaponSoundsCache.end())
    {
        UnloadSound(weaponSoundsCache[typeKey]);
        weaponSoundsCache.erase(typeKey);
    }
}

void AudioManager::ClearCache(ENEMY_TYPE type)
{
    int typeKey = static_cast<int>(type);
    if (enemySoundsCache.find(typeKey) != enemySoundsCache.end())
    {
        UnloadSound(enemySoundsCache[typeKey]);
        enemySoundsCache.erase(typeKey);
    }
}

void AudioManager::SetAssetsRoot(const std::string &assetsRoot)
{
    this->assetsRoot = assetsRoot;
    if (!this->assetsRoot.empty() && this->assetsRoot.back() != '/')
    {
        this->assetsRoot += '/';
    }
    ClearCache();
}

void AudioManager::DetectAndSetAssetsPath()
{
    // Buscar la carpeta de assets
    std::string currentDir = ".";

    // Intentar encontrar la carpeta de assets en diferentes ubicaciones
    while (currentDir.length() < 255)
    {
        std::string assetsPath = currentDir + "/assets";
        if (fs::exists(assetsPath) && fs::is_directory(assetsPath))
        {
            SetAssetsRoot(currentDir);
            TraceLog(LOG_INFO, "Assets encontrados en: %s", assetsPath.c_str());
            return;
        }
        currentDir += "/..";
    }

    TraceLog(LOG_WARNING, "No se encontró carpeta de assets, usando ruta por defecto");
}

std::string AudioManager::GetFilePath(PROJECTILE_TYPE type) const
{
    std::string basePath = assetsRoot.empty() ? BASE_PATH_PROJECTILE_SOUNDS : assetsRoot + BASE_PATH_PROJECTILE_SOUNDS;

    switch (type)
    {
    case PROJECTILE_TYPE::WAND:
        return basePath + "wand.wav";
    case PROJECTILE_TYPE::EGGPLOSIVE_CIRCLE:
        return basePath + "eggplosive_circle.wav";
    case PROJECTILE_TYPE::EGGPLOSIVE_BULLET:
        return basePath + "chicken.wav";
    case PROJECTILE_TYPE::LASER_RAY:
        return basePath + "laser_ray.wav";
    case PROJECTILE_TYPE::SNIPER:
        return basePath + "sniper.wav";
    case PROJECTILE_TYPE::WING:
        return basePath + "wing.wav";
    default:
        throw std::runtime_error("Tipo de proyectil desconocido");
    }
}

std::string AudioManager::GetFilePath(WEAPON_TYPE type) const
{
    std::string basePath = assetsRoot.empty() ? BASE_PATH_WEAPON_SOUNDS : assetsRoot + BASE_PATH_WEAPON_SOUNDS;

    switch (type)
    {
    case WEAPON_TYPE::AXE:
        return basePath + "axe.wav";
    case WEAPON_TYPE::SWORD:
        return basePath + "sword.wav";
    case WEAPON_TYPE::SCYTHE:
        return basePath + "scythe.wav";
    case WEAPON_TYPE::WAND:
        return basePath + "wand.wav";
    case WEAPON_TYPE::EGGPLOSIVE:
        return basePath + "eggplosive.wav";
    case WEAPON_TYPE::LASER_RAY:
        return basePath + "laser_ray.wav";
    case WEAPON_TYPE::SNIPER:
        return basePath + "sniper.wav";
    case WEAPON_TYPE::WING:
        return basePath + "wing.wav";
    default:
        throw std::runtime_error("Tipo de arma desconocida");
    }
}

const Sound &AudioManager::GetEnemySound(ENEMY_TYPE type)
{
    int typeKey = static_cast<int>(type);

    // Verificar si el sonido ya está en caché
    if (enemySoundsCache.find(typeKey) != enemySoundsCache.end())
    {
        return enemySoundsCache[typeKey];
    }

    // Cargar el sonido
    std::string filePath = GetFilePath(type);
    Sound sound = LoadSound(filePath.c_str());

    if (!IsSoundValid(sound))
    {
        throw std::runtime_error("No se pudo cargar el sonido: " + filePath);
    }

    enemySoundsCache[typeKey] = sound;
    return enemySoundsCache[typeKey];
}

void AudioManager::PlayEnemySound(ENEMY_TYPE type)
{
    try
    {
        const Sound &sound = GetEnemySound(type);
        int typeKey = static_cast<int>(type);
        
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
    // Detener música anterior si existe
    if (!currentMusicFile.empty())
    {
        StopBackgroundMusic();
    }

    // Cargar nueva música
    std::string filePath = assetsRoot.empty() ? BASE_PATH_MUSIC + musicFile : assetsRoot + BASE_PATH_MUSIC + musicFile;
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

void AudioManager::StopBackgroundMusic()
{
    if (!currentMusicFile.empty())
    {
        StopMusicStream(currentMusic);
        UnloadMusicStream(currentMusic);
        currentMusicFile = "";
    }
}

void AudioManager::PauseBackgroundMusic()
{
    if (!currentMusicFile.empty())
    {
        PauseMusicStream(currentMusic);
    }
}

void AudioManager::ResumeBackgroundMusic()
{
    if (!currentMusicFile.empty())
    {
        ResumeMusicStream(currentMusic);
    }
}

void AudioManager::SetMusicVolume(float volume)
{
    if (!currentMusicFile.empty())
    {
        currentMusicVolume = std::max(0.0f, std::min(1.0f, volume));
        ::SetMusicVolume(currentMusic, currentMusicVolume);
    }
}

float AudioManager::GetMusicVolume() const
{
    return currentMusicVolume;
}

bool AudioManager::IsMusicPlaying() const
{
    if (currentMusicFile.empty())
        return false;
    return IsMusicStreamPlaying(currentMusic);
}

std::string AudioManager::GetFilePath(ENEMY_TYPE type) const
{
    std::string basePath = assetsRoot.empty() ? BASE_PATH_ENEMY_SOUNDS : assetsRoot + BASE_PATH_ENEMY_SOUNDS;

    switch (type)
    {
        //esto cuando metamos a los siguientes enemigos lo cambiamos
    case ENEMY_TYPE::ZOMBIE:
        return basePath + "enemy.wav";
    case ENEMY_TYPE::ENEMY2:
        return basePath + "enemy2.wav";
    case ENEMY_TYPE::ENEMY3:
        return basePath + "enemy3.wav";
    case ENEMY_TYPE::ENEMY4:
        return basePath + "enemy4.wav";
    default:
        throw std::runtime_error("Tipo de enemigo desconocido");
    }
}
