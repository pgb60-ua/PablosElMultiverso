#define BOOST_TEST_MODULE AudioManagerTest
#include <boost/test/unit_test.hpp>

#include "AudioManager.hpp"
#include "Types.hpp"
#include <filesystem>
#include <vector>
#include <cstring>

extern "C"
{
    #include <raylib.h>
}

namespace fs = std::filesystem;

struct AudioManagerFixture
{
    fs::path testAssetsPath;

    AudioManagerFixture()
    {
        SetTraceLogLevel(LOG_NONE);
        SetConfigFlags(FLAG_WINDOW_HIDDEN);
        InitWindow(1, 1, "AudioTest");
        InitAudioDevice();

        testAssetsPath = fs::temp_directory_path() / "test_audio_assets";
        
        fs::create_directories(testAssetsPath / "assets" / "sounds" / "weapons");
        fs::create_directories(testAssetsPath / "assets" / "sounds" / "enemies");
        fs::create_directories(testAssetsPath / "assets" / "sounds" / "music");

        createTestWavFile(testAssetsPath / "assets" / "sounds" / "weapons" / "sword.wav");
        createTestWavFile(testAssetsPath / "assets" / "sounds" / "enemies" / "enemy.wav");
        createTestWavFile(testAssetsPath / "assets" / "sounds" / "music" / "background.wav");

        AudioManager::GetInstance().SetAssetsRoot(testAssetsPath.string());
    }

    ~AudioManagerFixture()
    {
        AudioManager::GetInstance().ClearCache();
        
        if (IsAudioDeviceReady()) CloseAudioDevice();
        if (IsWindowReady()) CloseWindow();

        if (fs::exists(testAssetsPath))
        {
            fs::remove_all(testAssetsPath);
        }
    }

    void createTestWavFile(const fs::path& filePath)
    {
        Wave wave = { 0 };
        wave.frameCount = 4410;
        wave.sampleRate = 44100;
        wave.sampleSize = 16;
        wave.channels = 1;
        
        int dataSize = wave.frameCount * wave.channels * (wave.sampleSize / 8);
        wave.data = malloc(dataSize);
        std::memset(wave.data, 0, dataSize);
        
        ExportWave(wave, filePath.string().c_str());
        
        free(wave.data);
    }
};

BOOST_FIXTURE_TEST_SUITE(AudioManagerTests, AudioManagerFixture)

BOOST_AUTO_TEST_CASE(GetSound_ValidType_ReturnsSound)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    const Sound& sound = AudioManager::GetInstance().GetSound(WEAPON_TYPE::SWORD);
    BOOST_CHECK(sound.frameCount > 0);
}

BOOST_AUTO_TEST_CASE(GetSound_InvalidFile_ThrowsException)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    BOOST_CHECK_THROW(AudioManager::GetInstance().GetSound(WEAPON_TYPE::AXE), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GetSound_CacheWorks)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    const Sound& sound1 = AudioManager::GetInstance().GetSound(WEAPON_TYPE::SWORD);
    const Sound& sound2 = AudioManager::GetInstance().GetSound(WEAPON_TYPE::SWORD);
    
    BOOST_CHECK_EQUAL(&sound1, &sound2);
}

BOOST_AUTO_TEST_CASE(GetEnemySound_ReturnsSound)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    const Sound& sound = AudioManager::GetInstance().GetEnemySound();
    BOOST_CHECK(sound.frameCount > 0);
}

BOOST_AUTO_TEST_CASE(PlayBackgroundMusic_LoadsMusic)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    
    BOOST_CHECK_NO_THROW(AudioManager::GetInstance().PlayBackgroundMusic("background.wav"));
}

BOOST_AUTO_TEST_CASE(ClearCache_ClearsEverything)
{
    if (!IsAudioDeviceReady())
    {
        BOOST_TEST_MESSAGE("Skipping test: Audio device not available");
        return;
    }
    AudioManager::GetInstance().GetSound(WEAPON_TYPE::SWORD);
    AudioManager::GetInstance().GetEnemySound();
    
    AudioManager::GetInstance().ClearCache();
    
    const Sound& soundNew = AudioManager::GetInstance().GetSound(WEAPON_TYPE::SWORD);
    
    BOOST_CHECK(soundNew.frameCount > 0);
}

BOOST_AUTO_TEST_SUITE_END()