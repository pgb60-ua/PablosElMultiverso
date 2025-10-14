#include "Player.hpp"
#include "SpriteLoaderManager.hpp"
#include "Types.hpp"
#include <MainGameState.hpp>
#include <iostream>

extern "C"
{
#include <raylib.h>
}

MainGameState::MainGameState() : direction{0, 0} {}

void MainGameState::init()
{
    // Crear el jugador en una posición inicial (ajusta según necesites)
    Vector2 initialPosition = {400.0f, 300.0f};
    player = std::make_unique<Player>(PLAYER_TYPE::RANGE, initialPosition);
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

    // Pasar la dirección al jugador para que la procese
    if (player)
    {
        player->HandleInput(direction);
    }
}

void MainGameState::update(float deltaTime)
{
    // Actualizar el jugador (esto llamará internamente a Move si hay dirección)
    if (player)
    {
        player->Update(deltaTime);
    }
}

void MainGameState::render()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Pablos El Multiverso", 10, 10, 20, DARKGRAY);

    // Renderizar el jugador
    if (player)
    {
        player->Render();
    }

    EndDrawing();
}
