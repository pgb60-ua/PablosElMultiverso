#define BOOST_TEST_MODULE DataFileManagerTest
#include <boost/test/unit_test.hpp>

#include "DataFileManager.hpp"
#include "Types.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

// Fixture para preparar el entorno de pruebas
struct DataFileManagerFixture
{
    DataFileManagerFixture()
    {
        // Crear directorio temporal para pruebas
        testAssetsPath = fs::temp_directory_path() / "test_assets";
        fs::create_directories(testAssetsPath / "data" / "players");
        fs::create_directories(testAssetsPath / "data" / "items");
        fs::create_directories(testAssetsPath / "data" / "enemies");
        fs::create_directories(testAssetsPath / "data" / "weapons");
        fs::create_directories(testAssetsPath / "data" / "rounds");

        // Crear archivos JSON de prueba
        createTestPlayerFile();
        createTestItemFile();
        createTestEnemyFile();
        createTestWeaponFile();
        createTestRoundFile();

        // Configurar el DataFileManager
        DataFileManager::GetInstance().SetAssetsRoot(testAssetsPath.string());
    }

    ~DataFileManagerFixture()
    {
        // Limpiar caché
        DataFileManager::GetInstance().ClearCache();
        
        // Eliminar archivos y directorios temporales
        if (fs::exists(testAssetsPath))
        {
            fs::remove_all(testAssetsPath);
        }
    }

    void createTestPlayerFile()
    {
        nlohmann::json j;
        j["health"] = 100;
        j["max_health"] = 100;
        j["physical_damage"] = 10;
        j["magical_damage"] = 0;
        j["armor"] = 5;
        j["resistance"] = 0;
        j["movement_speed"] = 1.5f;
        j["attack_speed"] = 1.0f;
        j["critical_chance"] = 0;
        j["critical_damage"] = 100;
        j["life_steal"] = 0;
        j["agility"] = 0;
        j["health_regeneration"] = 0;
        j["name"] = "Warrior";

        std::ofstream file(testAssetsPath / "data" / "players" / "warrior.json");
        file << j.dump(4);
        file.close();
    }

    void createTestItemFile()
    {
        nlohmann::json j;
        j["name"] = "Fireball";
        j["description"] = "A magical fireball";
        j["rarity"] = "COMMON";
        j["price"] = 100;
        j["physical_damage"] = 20;
        j["magical_damage"] = 0;
        j["health"] = 0;
        j["max_health"] = 0;
        j["armor"] = 0;
        j["resistance"] = 0;
        j["movement_speed"] = 0.0f;
        j["attack_speed"] = 1.0f;
        j["critical_chance"] = 0;
        j["critical_damage"] = 100;
        j["life_steal"] = 0;
        j["agility"] = 0;
        j["health_regeneration"] = 0;

        std::ofstream file(testAssetsPath / "data" / "items" / "Fireball.json");
        file << j.dump(4);
        file.close();
    }

    void createTestEnemyFile()
    {
        nlohmann::json j;
        j["health"] = 50;
        j["max_health"] = 50;
        j["physical_damage"] = 15;
        j["magical_damage"] = 0;
        j["armor"] = 3;
        j["resistance"] = 0;
        j["movement_speed"] = 1.0f;
        j["attack_speed"] = 1.0f;
        j["critical_chance"] = 0;
        j["critical_damage"] = 100;
        j["life_steal"] = 0;
        j["agility"] = 0;
        j["health_regeneration"] = 0;
        j["name"] = "Zombie";

        std::ofstream file(testAssetsPath / "data" / "enemies" / "zombie.json");
        file << j.dump(4);
        file.close();
    }

    void createTestWeaponFile()
    {
        nlohmann::json j;
        j["health"] = 0;
        j["max_health"] = 0;
        j["physical_damage"] = 25;
        j["magical_damage"] = 0;
        j["armor"] = 0;
        j["resistance"] = 0;
        j["movement_speed"] = 0.0f;
        j["attack_speed"] = 1.0f;
        j["critical_chance"] = 0;
        j["critical_damage"] = 100;
        j["life_steal"] = 0;
        j["agility"] = 0;
        j["health_regeneration"] = 0;
        j["name"] = "Axe";

        std::ofstream file(testAssetsPath / "data" / "weapons" / "axe.json");
        file << j.dump(4);
        file.close();
    }

    void createTestRoundFile()
    {
        nlohmann::json j;
        j["rounds"] = nlohmann::json::array();
        
        nlohmann::json round1;
        round1["roundNumber"] = 1;
        round1["duration"] = 60.0;
        round1["spawnRate"] = 1.0;
        round1["enemiesToSpawn"] = {
            {"ZOMBIE", 10}
        };
        j["rounds"].push_back(round1);

        std::ofstream file(testAssetsPath / "data" / "rounds" / "easy.json");
        file << j.dump(4);
        file.close();
    }

    fs::path testAssetsPath;
};

BOOST_FIXTURE_TEST_SUITE(DataFileManagerTestSuite, DataFileManagerFixture)

// Test 1: Verificar que GetInstance devuelve una única instancia (Singleton)
BOOST_AUTO_TEST_CASE(test_singleton_instance)
{
    DataFileManager &instance1 = DataFileManager::GetInstance();
    DataFileManager &instance2 = DataFileManager::GetInstance();

    BOOST_CHECK_EQUAL(&instance1, &instance2);
}

// Test 2: Verificar SetAssetsRoot actualiza las rutas correctamente
BOOST_AUTO_TEST_CASE(test_set_assets_root)
{
    std::string newRoot = "/custom/path/";
    DataFileManager::GetInstance().SetAssetsRoot(newRoot);

    // Intentar cargar datos con la nueva ruta debería fallar si no existen
    BOOST_CHECK_THROW(DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR), std::exception);

    // Restaurar la ruta de prueba
    DataFileManager::GetInstance().SetAssetsRoot(testAssetsPath.string());
}

// Test 3: Verificar carga de datos de jugador
BOOST_AUTO_TEST_CASE(test_get_player_data)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK(!data.empty());
    BOOST_CHECK(data.find("health") != data.end());
    BOOST_CHECK_EQUAL(std::get<int>(data.at("health")), 100);
    BOOST_CHECK_EQUAL(std::get<int>(data.at("physical_damage")), 10);
}

// Test 4: Verificar carga de Stats de jugador
BOOST_AUTO_TEST_CASE(test_get_player_stats)
{
    Stats stats = DataFileManager::GetInstance().GetPlayerStats(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK_EQUAL(stats.GetHealth(), 100);
    BOOST_CHECK_EQUAL(stats.GetPhysicalDamage(), 10);
    BOOST_CHECK_EQUAL(stats.GetArmor(), 5);
    BOOST_CHECK_CLOSE(stats.GetMovementSpeed(), 1.5f, 0.01f);
}

// Test 5: Verificar carga de datos de item
BOOST_AUTO_TEST_CASE(test_get_item_data)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL);

    BOOST_CHECK(!data.empty());
    BOOST_CHECK(data.find("name") != data.end());
    BOOST_CHECK_EQUAL(std::get<std::string>(data.at("name")), "Fireball");
}

// Test 6: Verificar carga de ItemData completo
BOOST_AUTO_TEST_CASE(test_get_item_full_data)
{
    ItemData itemData = DataFileManager::GetInstance().GetItemData(ITEM_TYPE::FIREBALL);

    BOOST_CHECK_EQUAL(itemData.name, "Fireball");
    BOOST_CHECK_EQUAL(itemData.description, "A magical fireball");
    BOOST_CHECK_EQUAL(itemData.price, 100);
    BOOST_CHECK_EQUAL(itemData.stats.GetPhysicalDamage(), 20);
}

// Test 7: Verificar carga de Stats de item
BOOST_AUTO_TEST_CASE(test_get_item_stats)
{
    Stats stats = DataFileManager::GetInstance().GetItemStats(ITEM_TYPE::FIREBALL);

    BOOST_CHECK_EQUAL(stats.GetHealth(), 0);
    BOOST_CHECK_EQUAL(stats.GetPhysicalDamage(), 20);
    BOOST_CHECK_EQUAL(stats.GetArmor(), 0);
}

// Test 8: Verificar carga de datos de enemigo
BOOST_AUTO_TEST_CASE(test_get_enemy_data)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(ENEMY_TYPE::ZOMBIE);

    BOOST_CHECK(!data.empty());
    BOOST_CHECK(data.find("health") != data.end());
    BOOST_CHECK_EQUAL(std::get<int>(data.at("health")), 50);
}

// Test 9: Verificar carga de Stats de enemigo
BOOST_AUTO_TEST_CASE(test_get_enemy_stats)
{
    Stats stats = DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::ZOMBIE);

    BOOST_CHECK_EQUAL(stats.GetHealth(), 50);
    BOOST_CHECK_EQUAL(stats.GetPhysicalDamage(), 15);
    BOOST_CHECK_EQUAL(stats.GetArmor(), 3);
}

// Test 10: Verificar carga de datos de arma
BOOST_AUTO_TEST_CASE(test_get_weapon_data)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(WEAPON_TYPE::AXE);

    BOOST_CHECK(!data.empty());
    BOOST_CHECK(data.find("physical_damage") != data.end());
    BOOST_CHECK_EQUAL(std::get<int>(data.at("physical_damage")), 25);
}

// Test 11: Verificar carga de Stats de arma
BOOST_AUTO_TEST_CASE(test_get_weapon_stats)
{
    Stats stats = DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::AXE);

    BOOST_CHECK_EQUAL(stats.GetPhysicalDamage(), 25);
    BOOST_CHECK_EQUAL(stats.GetHealth(), 0);
}

// Test 12: Verificar carga de datos de ronda
BOOST_AUTO_TEST_CASE(test_get_round_data)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(ROUND_TYPE::EASY);

    BOOST_CHECK(!data.empty());
}

// Test 13: Verificar carga de información de rondas
BOOST_AUTO_TEST_CASE(test_get_rounds)
{
    std::vector<RoundInfo> rounds = DataFileManager::GetInstance().GetRounds(ROUND_TYPE::EASY);

    BOOST_CHECK(!rounds.empty());
    BOOST_CHECK_EQUAL(rounds.size(), 1);
}

// Test 14: Verificar limpieza de caché
BOOST_AUTO_TEST_CASE(test_clear_cache)
{
    // Cargar algunos datos para llenar el caché
    DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL);

    // Limpiar todo el caché
    DataFileManager::GetInstance().ClearCache();

    // Los datos deberían cargarse nuevamente (no hay forma directa de verificar 
    // el caché, pero podemos verificar que no se lanza excepción)
    BOOST_CHECK_NO_THROW(DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR));
}

// Test 15: Verificar limpieza de caché por tipo - Players
BOOST_AUTO_TEST_CASE(test_clear_cache_players)
{
    DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    DataFileManager::GetInstance().ClearCachePlayers();
    
    BOOST_CHECK_NO_THROW(DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR));
}

// Test 16: Verificar limpieza de caché por tipo - Items
BOOST_AUTO_TEST_CASE(test_clear_cache_items)
{
    DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL);
    DataFileManager::GetInstance().ClearCacheItems();
    
    BOOST_CHECK_NO_THROW(DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL));
}

// Test 17: Verificar limpieza de caché por tipo específico - Player
BOOST_AUTO_TEST_CASE(test_clear_cache_specific_player)
{
    DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    DataFileManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);
    
    BOOST_CHECK_NO_THROW(DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR));
}

// Test 18: Verificar limpieza de caché por tipo específico - Item
BOOST_AUTO_TEST_CASE(test_clear_cache_specific_item)
{
    DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL);
    DataFileManager::GetInstance().ClearCache(ITEM_TYPE::FIREBALL);
    
    BOOST_CHECK_NO_THROW(DataFileManager::GetInstance().GetData(ITEM_TYPE::FIREBALL));
}

// Test 19: Verificar que se lanza excepción con archivo inexistente
BOOST_AUTO_TEST_CASE(test_load_nonexistent_file)
{
    // Eliminar el archivo de prueba
    fs::remove(testAssetsPath / "data" / "players" / "warrior.json");
    DataFileManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK_THROW(DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR), std::exception);

    // Recrear el archivo para otros tests
    createTestPlayerFile();
}

// Test 20: Verificar que se puede recargar datos después de limpiar caché
BOOST_AUTO_TEST_CASE(test_reload_after_cache_clear)
{
    // Primera carga
    const DataMap &data1 = DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    int health1 = std::get<int>(data1.at("health"));

    // Modificar archivo
    nlohmann::json j;
    j["health"] = 200;
    j["max_health"] = 200;
    j["physical_damage"] = 20;
    j["magical_damage"] = 0;
    j["armor"] = 10;
    j["resistance"] = 0;
    j["movement_speed"] = 2.0f;
    j["attack_speed"] = 1.0f;
    j["critical_chance"] = 0;
    j["critical_damage"] = 100;
    j["life_steal"] = 0;
    j["agility"] = 0;
    j["health_regeneration"] = 0;
    j["name"] = "Warrior";

    std::ofstream file(testAssetsPath / "data" / "players" / "warrior.json");
    file << j.dump(4);
    file.close();

    // Sin limpiar caché, debería tener el valor antiguo
    const DataMap &data2 = DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    int health2 = std::get<int>(data2.at("health"));
    BOOST_CHECK_EQUAL(health1, health2);

    // Limpiar caché y recargar
    DataFileManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);
    const DataMap &data3 = DataFileManager::GetInstance().GetData(PLAYER_TYPE::WARRIOR);
    int health3 = std::get<int>(data3.at("health"));

    // Ahora debería tener el nuevo valor
    BOOST_CHECK_EQUAL(health3, 200);

    // Restaurar archivo original
    createTestPlayerFile();
    DataFileManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);
}

BOOST_AUTO_TEST_SUITE_END()
