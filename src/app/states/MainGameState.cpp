#include "Player.hpp"
#include "SpriteLoaderManager.hpp"
#include "Types.hpp"
#include "Zombie.hpp"
#include "WingWeapon.hpp"
#include <MainGameState.hpp>
#include <iostream>

extern "C"
{
#include <raylib.h>
}

MainGameState::MainGameState() : direction{0, 0} {}

void MainGameState::init()
{
    // Crear el jugador en una posición inicial
    Vector2 initialPosition = {400.0f, 300.0f};
    Vector2 secondPosition = {600.0f, 700.0f};
    players.push_back(std::make_unique<Player>(PLAYER_TYPE::RANGE, initialPosition));
    players.push_back(std::make_unique<Player>(PLAYER_TYPE::MAGE, secondPosition));

    for (int i = 0; i < 10; i++)
    {
        enemies.push_back(new Zombie(std::vector<Player *>{players[0].get(), players[1].get()}));
    }

    // Crear el arma desde JSON automáticamente en el constructor
    currentWeapon = new WingWeapon(Vector2{400.0f, 300.0f}, enemies);
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
    players[1]->HandleInput(direction2);
}

void MainGameState::update(float deltaTime)
{
    // Actualizar todos los jugadores (esto llamará internamente a Move si hay dirección)
    for (auto &player : players)
    {
        player->Update(deltaTime);
    }
    // Actualizar todos los enemigos
    for (auto &enemy : enemies)
    {
        enemy->Update(deltaTime);
    }
    if(currentWeapon)
    {
        // Asumimos que el arma sigue al primer jugador
        Vector2 playerPos = {players[0]->GetPosition().x + 32 + 16, players[0]->GetPosition().y - 32 - 16};
        currentWeapon->update(deltaTime, playerPos);
    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(DARKGRAY);
    DrawText("Pablos El Multiverso", 10, 10, 20, LIGHTGRAY);

    // Renderizar todos los jugadores
    for (auto &player : players)
    {
        player->Render();
    }
    // Renderizar todos los enemigos
    for (auto &enemy : enemies)
    {
        enemy->Render();
    }
    if(currentWeapon)
    {
        currentWeapon->render();
    }
    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
}

MainGameState::~MainGameState()
{
    if(currentWeapon)
    {
        delete currentWeapon;
        currentWeapon = nullptr;
    }
}