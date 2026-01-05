#include "ShopState.hpp"
#include "MainGameState.hpp"
#include "StateMachine.hpp"
#include "raylib.h"
#include <memory>

ShopState::ShopState() {}
ShopState::~ShopState() {}
void ShopState::init() {}
void ShopState::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        passRound = true;
    }
}
void ShopState::update(float deltaTime)
{
    if (passRound)
    {
        state_machine->remove_state(false);
    }
}
void ShopState::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("Prueba de tienda", 200, 200, 20, WHITE);

    EndDrawing();
}
