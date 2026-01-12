#define BOOST_TEST_MODULE SpriteLoaderManagerTest
#include <boost/test/unit_test.hpp>

#include "SpriteLoaderManager.hpp"
#include "Types.hpp"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

extern "C"
{
    #include <raylib.h>
}

namespace fs = std::filesystem;

// Fixture para preparar el entorno de pruebas
struct SpriteLoaderManagerFixture
{
    SpriteLoaderManagerFixture()
    {
        // Inicializar Raylib con una ventana oculta/minimizada
        SetTraceLogLevel(LOG_NONE); // Desactivar logs de Raylib
        SetConfigFlags(FLAG_WINDOW_HIDDEN); // Ventana oculta
        InitWindow(1, 1, "Test"); // Ventana mínima
        
        // Crear directorio temporal para pruebas
        testAssetsPath = fs::temp_directory_path() / "test_sprites_assets";
        fs::create_directories(testAssetsPath / "sprites" / "players");
        fs::create_directories(testAssetsPath / "sprites" / "items");
        fs::create_directories(testAssetsPath / "sprites" / "enemies");
        fs::create_directories(testAssetsPath / "sprites" / "projectiles");
        fs::create_directories(testAssetsPath / "sprites" / "weapons");
        fs::create_directories(testAssetsPath / "sprites" / "maps");

        // Crear archivos JSON y texturas de prueba
        createTestPlayerSprite();
        createTestItemSprite();
        createTestEnemySprite();
        createTestProjectileSprite();
        createTestWeaponSprite();

        // Configurar el SpriteLoaderManager
        SpriteLoaderManager::GetInstance().SetAssetsRoot(testAssetsPath.string());
    }

    ~SpriteLoaderManagerFixture()
    {
        // Limpiar caché
        SpriteLoaderManager::GetInstance().ClearCache();
        
        // Eliminar archivos y directorios temporales
        if (fs::exists(testAssetsPath))
        {
            fs::remove_all(testAssetsPath);
        }
        
        // Cerrar ventana de Raylib
        CloseWindow();
    }

    void createTestPlayerSprite()
    {
        // Crear JSON de metadatos
        nlohmann::json j;
        j["texture"] = "warrior.png";
        j["frames"] = nlohmann::json::array();
        
        nlohmann::json frame1;
        frame1["x"] = 0;
        frame1["y"] = 0;
        frame1["width"] = 32;
        frame1["height"] = 32;
        j["frames"].push_back(frame1);
        
        nlohmann::json frame2;
        frame2["x"] = 32;
        frame2["y"] = 0;
        frame2["width"] = 32;
        frame2["height"] = 32;
        j["frames"].push_back(frame2);

        std::ofstream file(testAssetsPath / "sprites" / "players" / "warrior.json");
        file << j.dump(4);
        file.close();

        // Crear textura de prueba simple (1x1 pixel)
        Image img = GenImageColor(64, 32, RED);
        ExportImage(img, (testAssetsPath / "sprites" / "players" / "warrior.png").string().c_str());
        UnloadImage(img);
    }

    void createTestItemSprite()
    {
        nlohmann::json j;
        j["texture"] = "Fireball.png";
        j["frames"] = nlohmann::json::array();
        
        nlohmann::json frame1;
        frame1["x"] = 0;
        frame1["y"] = 0;
        frame1["width"] = 16;
        frame1["height"] = 16;
        j["frames"].push_back(frame1);

        std::ofstream file(testAssetsPath / "sprites" / "items" / "Fireball.json");
        file << j.dump(4);
        file.close();

        Image img = GenImageColor(16, 16, ORANGE);
        ExportImage(img, (testAssetsPath / "sprites" / "items" / "Fireball.png").string().c_str());
        UnloadImage(img);
    }

    void createTestEnemySprite()
    {
        nlohmann::json j;
        j["texture"] = "zombie.png";
        j["frames"] = nlohmann::json::array();
        
        nlohmann::json frame1;
        frame1["x"] = 0;
        frame1["y"] = 0;
        frame1["width"] = 32;
        frame1["height"] = 32;
        j["frames"].push_back(frame1);

        std::ofstream file(testAssetsPath / "sprites" / "enemies" / "zombie.json");
        file << j.dump(4);
        file.close();

        Image img = GenImageColor(32, 32, GREEN);
        ExportImage(img, (testAssetsPath / "sprites" / "enemies" / "zombie.png").string().c_str());
        UnloadImage(img);
    }

    void createTestProjectileSprite()
    {
        nlohmann::json j;
        j["texture"] = "wing.png";
        j["frames"] = nlohmann::json::array();
        
        nlohmann::json frame1;
        frame1["x"] = 0;
        frame1["y"] = 0;
        frame1["width"] = 8;
        frame1["height"] = 8;
        j["frames"].push_back(frame1);

        std::ofstream file(testAssetsPath / "sprites" / "projectiles" / "wing.json");
        file << j.dump(4);
        file.close();

        Image img = GenImageColor(8, 8, YELLOW);
        ExportImage(img, (testAssetsPath / "sprites" / "projectiles" / "wing.png").string().c_str());
        UnloadImage(img);
    }

    void createTestWeaponSprite()
    {
        nlohmann::json j;
        j["texture"] = "axe.png";
        j["frames"] = nlohmann::json::array();
        
        nlohmann::json frame1;
        frame1["x"] = 0;
        frame1["y"] = 0;
        frame1["width"] = 24;
        frame1["height"] = 24;
        j["frames"].push_back(frame1);

        std::ofstream file(testAssetsPath / "sprites" / "weapons" / "axe.json");
        file << j.dump(4);
        file.close();

        Image img = GenImageColor(24, 24, GRAY);
        ExportImage(img, (testAssetsPath / "sprites" / "weapons" / "axe.png").string().c_str());
        UnloadImage(img);
    }

    fs::path testAssetsPath;
};

BOOST_FIXTURE_TEST_SUITE(SpriteLoaderManagerTestSuite, SpriteLoaderManagerFixture)

// Test 1: Verificar que GetInstance devuelve una única instancia (Singleton)
BOOST_AUTO_TEST_CASE(test_singleton_instance)
{
    SpriteLoaderManager &instance1 = SpriteLoaderManager::GetInstance();
    SpriteLoaderManager &instance2 = SpriteLoaderManager::GetInstance();

    BOOST_CHECK_EQUAL(&instance1, &instance2);
}

// Test 2: Verificar SetAssetsRoot actualiza las rutas correctamente
BOOST_AUTO_TEST_CASE(test_set_assets_root)
{
    std::string newRoot = "/custom/path/";
    SpriteLoaderManager::GetInstance().SetAssetsRoot(newRoot);

    // Verificar que las rutas base se actualizaron
    BOOST_CHECK_EQUAL(SpriteLoaderManager::GetInstance().BASE_PATH_PLAYER, "/custom/path/sprites/players/");
    BOOST_CHECK_EQUAL(SpriteLoaderManager::GetInstance().BASE_PATH_ITEM, "/custom/path/sprites/items/");
    
    // Restaurar la ruta de prueba
    SpriteLoaderManager::GetInstance().SetAssetsRoot(testAssetsPath.string());
}

// Test 3: Verificar carga de SpriteSheet de jugador
BOOST_AUTO_TEST_CASE(test_get_player_spritesheet)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK(sheet.texture.id > 0); // Textura válida
    BOOST_CHECK_EQUAL(sheet.frames.size(), 2); // 2 frames creados
    BOOST_CHECK_EQUAL(sheet.frames[0].width, 32);
    BOOST_CHECK_EQUAL(sheet.frames[0].height, 32);
}

// Test 4: Verificar carga de SpriteSheet de item
BOOST_AUTO_TEST_CASE(test_get_item_spritesheet)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL);

    BOOST_CHECK(sheet.texture.id > 0);
    BOOST_CHECK_EQUAL(sheet.frames.size(), 1);
    BOOST_CHECK_EQUAL(sheet.frames[0].width, 16);
    BOOST_CHECK_EQUAL(sheet.frames[0].height, 16);
}

// Test 5: Verificar carga de SpriteSheet de enemigo
BOOST_AUTO_TEST_CASE(test_get_enemy_spritesheet)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);

    BOOST_CHECK(sheet.texture.id > 0);
    BOOST_CHECK_EQUAL(sheet.frames.size(), 1);
    BOOST_CHECK_EQUAL(sheet.frames[0].width, 32);
    BOOST_CHECK_EQUAL(sheet.frames[0].height, 32);
}

// Test 6: Verificar carga de SpriteSheet de proyectil
BOOST_AUTO_TEST_CASE(test_get_projectile_spritesheet)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::WING);

    BOOST_CHECK(sheet.texture.id > 0);
    BOOST_CHECK_EQUAL(sheet.frames.size(), 1);
    BOOST_CHECK_EQUAL(sheet.frames[0].width, 8);
    BOOST_CHECK_EQUAL(sheet.frames[0].height, 8);
}

// Test 7: Verificar carga de SpriteSheet de arma
BOOST_AUTO_TEST_CASE(test_get_weapon_spritesheet)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::AXE);

    BOOST_CHECK(sheet.texture.id > 0);
    BOOST_CHECK_EQUAL(sheet.frames.size(), 1);
    BOOST_CHECK_EQUAL(sheet.frames[0].width, 24);
    BOOST_CHECK_EQUAL(sheet.frames[0].height, 24);
}

// Test 8: Verificar obtención de hitbox de jugador
BOOST_AUTO_TEST_CASE(test_get_player_hitbox)
{
    Vector2 pos = {100.0f, 100.0f};
    Shape hitbox = SpriteLoaderManager::GetInstance().GetSpriteHitbox(PLAYER_TYPE::WARRIOR, pos);

    // Verificar que el hitbox es un rectángulo
    BOOST_CHECK_EQUAL(hitbox.type, SHAPE_RECTANGLE);
    
    // Verificar dimensiones del frame (32x32)
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.width, 32.0f, 0.01f);
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.height, 32.0f, 0.01f);
}

// Test 9: Verificar obtención de hitbox de enemigo
BOOST_AUTO_TEST_CASE(test_get_enemy_hitbox)
{
    Vector2 pos = {200.0f, 200.0f};
    Shape hitbox = SpriteLoaderManager::GetInstance().GetSpriteHitbox(ENEMY_TYPE::ZOMBIE, pos);

    BOOST_CHECK_EQUAL(hitbox.type, SHAPE_RECTANGLE);
    
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.width, 32.0f, 0.01f);
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.height, 32.0f, 0.01f);
}

// Test 10: Verificar obtención de hitbox de proyectil
BOOST_AUTO_TEST_CASE(test_get_projectile_hitbox)
{
    Vector2 pos = {50.0f, 50.0f};
    Shape hitbox = SpriteLoaderManager::GetInstance().GetSpriteHitbox(PROJECTILE_TYPE::WING, pos);

    BOOST_CHECK_EQUAL(hitbox.type, SHAPE_RECTANGLE);
    
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.width, 8.0f, 0.01f);
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.height, 8.0f, 0.01f);
}

// Test 11: Verificar obtención de hitbox de arma
BOOST_AUTO_TEST_CASE(test_get_weapon_hitbox)
{
    Vector2 pos = {150.0f, 150.0f};
    Shape hitbox = SpriteLoaderManager::GetInstance().GetSpriteHitbox(WEAPON_TYPE::AXE, pos);

    BOOST_CHECK_EQUAL(hitbox.type, SHAPE_RECTANGLE);
    
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.width, 24.0f, 0.01f);
    BOOST_CHECK_CLOSE(hitbox.data.rectangle.height, 24.0f, 0.01f);
}

// Test 12: Verificar caché - segunda carga debe usar caché
BOOST_AUTO_TEST_CASE(test_cache_usage)
{
    const SpriteSheet &sheet1 = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);
    const SpriteSheet &sheet2 = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);

    // Deben ser la misma referencia (caché)
    BOOST_CHECK_EQUAL(&sheet1, &sheet2);
}

// Test 13: Verificar limpieza de caché completa
BOOST_AUTO_TEST_CASE(test_clear_cache)
{
    // Cargar algunos sprites
    SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);
    SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL);

    // Limpiar todo el caché
    SpriteLoaderManager::GetInstance().ClearCache();

    // Después de limpiar, debería poder cargar de nuevo sin errores
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR));
}

// Test 14: Verificar limpieza de caché por categoría - Players
BOOST_AUTO_TEST_CASE(test_clear_cache_players)
{
    SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);
    SpriteLoaderManager::GetInstance().ClearCachePlayers();
    
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR));
}

// Test 15: Verificar limpieza de caché por categoría - Items
BOOST_AUTO_TEST_CASE(test_clear_cache_items)
{
    SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL);
    SpriteLoaderManager::GetInstance().ClearCacheItems();
    
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL));
}

// Test 16: Verificar limpieza de caché por categoría - Enemies
BOOST_AUTO_TEST_CASE(test_clear_cache_enemies)
{
    SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);
    SpriteLoaderManager::GetInstance().ClearCacheEnemies();
    
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE));
}

// Test 17: Verificar limpieza de caché por tipo específico - Player
BOOST_AUTO_TEST_CASE(test_clear_cache_specific_player)
{
    SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);
    SpriteLoaderManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);
    
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR));
}

// Test 18: Verificar limpieza de caché por tipo específico - Item
BOOST_AUTO_TEST_CASE(test_clear_cache_specific_item)
{
    SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL);
    SpriteLoaderManager::GetInstance().ClearCache(ITEM_TYPE::FIREBALL);
    
    BOOST_CHECK_NO_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::FIREBALL));
}

// Test 19: Verificar que se lanza excepción con archivo inexistente
BOOST_AUTO_TEST_CASE(test_load_nonexistent_file)
{
    // Eliminar el archivo de prueba
    fs::remove(testAssetsPath / "sprites" / "players" / "warrior.json");
    fs::remove(testAssetsPath / "sprites" / "players" / "warrior.png");
    SpriteLoaderManager::GetInstance().ClearCache(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK_THROW(SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR), std::exception);

    // Recrear el archivo para otros tests
    createTestPlayerSprite();
}

// Test 20: Verificar que dimensiones de textura son correctas
BOOST_AUTO_TEST_CASE(test_texture_dimensions)
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PLAYER_TYPE::WARRIOR);

    BOOST_CHECK_EQUAL(sheet.texture.width, 64);
    BOOST_CHECK_EQUAL(sheet.texture.height, 32);
}

BOOST_AUTO_TEST_SUITE_END()
