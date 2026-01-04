#include "Player.hpp"
#include "SpriteLoaderManager.hpp"
#include "Types.hpp"
#include "WingWeapon.hpp"
#include "LaserRayWeapon.hpp"
#include "SniperWeapon.hpp"
#include "EggplosiveWeapon.hpp"
#include "GameOverState.hpp"
#include "GameWonState.hpp"
#include "StateMachine.hpp"
#include "Zombie.hpp"
#include <MainGameState.hpp>

extern "C"
{
#include <raylib.h>
}

// Constructor que recibe el tipo de jugador
MainGameState::MainGameState(PLAYER_TYPE playerType)
    : selectedPlayerType(playerType), direction{0, 0}
{
}

// Constructor por defecto con tipo de jugador por defecto (Mage)
MainGameState::MainGameState()
    : selectedPlayerType(PLAYER_TYPE::MAGE), direction{0, 0} // Valor por defecto
{
}

void MainGameState::init()
{  
    // Crear el jugador en una posición inicial
    Vector2 initialPosition = {400.0f, 300.0f};
    players.push_back(std::make_unique<Player>(selectedPlayerType, initialPosition, enemies));

    int numZombies = 100;
    enemies.reserve(numZombies);
    for (int i = 0; i < numZombies; i++)
    {
        enemies.push_back(new Zombie(std::vector<Player *>{players[0].get()}));
    }

    // Crear el arma desde JSON automáticamente en el constructor

    players[0]->AddWeapon(std::make_unique<LaserRayWeapon>(Vector2{400.0f, 300.0f}, enemies, enemies));
    players[0]->AddWeapon(std::make_unique<SniperWeapon>(Vector2{400.0f, 300.0f}, enemies, enemies));
    players[0]->AddWeapon(std::make_unique<WingWeapon>(Vector2{400.0f, 300.0f}, enemies, enemies));
    players[0]->AddWeapon(std::make_unique<EggplosiveWeapon>(Vector2{400.0f, 300.0f}, enemies, enemies));

}

void MainGameState::handleInput()
{
    // Resetear la dirección
    direction = {0, 0};

    // Detectar input de teclas y construir vector de dirección
    if (IsKeyDown(KEY_W))
    {
        direction.y = -1; // Arriba (Y negativa en Raylib)
    }
    if (IsKeyDown(KEY_S))
    {
        direction.y = 1; // Abajo (Y positiva en Raylib)
    }
    if (IsKeyDown(KEY_A))
    {
        direction.x = -1; // Izquierda (X negativa)
    }
    if (IsKeyDown(KEY_D))
    {
        direction.x = 1; // Derecha (X positiva)
    }
    // Resetear la dirección
    direction2 = {0, 0};

    // Detectar input de teclas y construir vector de dirección
    if (IsKeyDown(KEY_UP))
    {
        direction2.y = -1; // Arriba (Y negativa en Raylib)
    }
    if (IsKeyDown(KEY_DOWN))
    {
        direction2.y = 1; // Abajo (Y positiva en Raylib)
    }
    if (IsKeyDown(KEY_LEFT))
    {
        direction2.x = -1; // Izquierda (X negativa)
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        direction2.x = 1; // Derecha (X positiva)
    }

    // Pasar la dirección a todos los jugadores para que la procesen
    /*for (auto &player : players)
    {
        player->HandleInput(direction);
    }*/
    players[0]->HandleInput(direction);
}

void MainGameState::update(float deltaTime)
{
    int numero_vivo = 0;
    // Actualizar todos los jugadores (esto llamará internamente a Move si hay dirección)
    for (auto &player : players)
    {
        player->Update(deltaTime);
        player->CheckCollisions(deltaTime);
        if (!player->IsAlive())
        {
            numero_vivo++;
        }

    }
    // Actualizar todos los enemigos
    for (auto &enemy : enemies)
    {
        enemy->Update(deltaTime);
    }
    if (numero_vivo == (int)players.size())
    {
        // Todos los jugadores están muertos, reiniciar el estado del juego
        state_machine->add_state(std::make_unique<GameOverState>(), true);
    }
    else if (enemies.empty())
    {
        state_machine->add_state(std::make_unique<GameWonState>(), true);
    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);

    const SpriteSheet &mapSprite = SpriteLoaderManager::GetInstance().GetSpriteSheet(MAP_TYPE::DEFAULT);
    DrawTextureRec(mapSprite.texture, mapSprite.frames[0], {0, 0}, WHITE);
    
    DrawText("Pablos El Multiverso", 10, 10, 20, LIGHTGRAY);

    // Renderizar todos los jugadores
    for (auto &player : players)
    {
        player->Render();
        std::string healthText = "Health: " + std::to_string(static_cast<int>(player->GetHealth()));
        DrawText(healthText.c_str(), static_cast<int>(player->GetPosition().x),
                 static_cast<int>(player->GetPosition().y) + 64, 10, GREEN);
    }
    // Renderizar todos los enemigos
    for (auto &enemy : enemies)
    {
        enemy->Render();
    }
    DrawFPS(GetScreenWidth() - 100, 10);

    const SpriteSheet &mapUpperSprite = SpriteLoaderManager::GetInstance().GetSpriteSheet(MAP_TYPE::DEFAULT_UPPER);
    DrawTextureRec(mapUpperSprite.texture, mapUpperSprite.frames[0], {0, 0}, WHITE);
    EndDrawing();
}

MainGameState::~MainGameState()
{
    for (auto &enemy : enemies)
    {
        delete enemy;
    }
}
