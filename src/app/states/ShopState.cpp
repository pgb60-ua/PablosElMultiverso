#include "ShopState.hpp"
#include "MainGameState.hpp"
#include "Player.hpp"
#include "StateMachine.hpp"
#include "raylib.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>

ShopState::ShopState(Player *player) : player(player) {}
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
    int screenSemiWidth = GetScreenWidth() / 2;
    int heightStats = GetScreenHeight() - 200;
    int widthStats = screenSemiWidth - GetScreenWidth() / 6;

    // Entre 13 porque muestro 12 stats y dejo 1 para margen de arriba y abajo
    int spaceStats = heightStats / 13;

    BeginDrawing();

    ClearBackground(BLACK);

    // Pintamos las stats
    DrawRectangle(100, 100, widthStats, heightStats, WHITE);

    DrawText(TextFormat("Max Health: %.1f", player->GetMaxHealth()), 120, PADDING + spaceStats, 20, RED);
    DrawText(TextFormat("Movement speed: %.1f", player->GetMovementSpeed()), 120, PADDING + spaceStats * 2, 20, RED);
    DrawText(TextFormat("Agility: %.1f", player->GetAgility()), 120, PADDING + spaceStats * 3, 20, RED);
    DrawText(TextFormat("Attack Speed: %.1f", player->GetAttackSpeed()), 120, PADDING + spaceStats * 4, 20, RED);
    DrawText(TextFormat("Physical Damage: %.1f", player->GetPhysicalDamage()), 120, PADDING + spaceStats * 5, 20, RED);
    DrawText(TextFormat("Magical Damage: %.1f", player->GetMagicDamage()), 120, PADDING + spaceStats * 6, 20, RED);
    DrawText(TextFormat("Resistance: %.1f", player->GetResistance()), 120, PADDING + spaceStats * 7, 20, RED);
    DrawText(TextFormat("Armor: %.1f", player->GetArmor()), 120, PADDING + spaceStats * 8, 20, RED);
    DrawText(TextFormat("Critical Chance: %.1f", player->GetCriticalChance()), 120, PADDING + spaceStats * 9, 20, RED);
    DrawText(TextFormat("Critical Damage: %.1f", player->GetCriticalDamage()), 120, PADDING + spaceStats * 10, 20, RED);
    DrawText(TextFormat("Health Regeneration: %.1f", player->GetHealthRegeneration()), 120, PADDING + spaceStats * 11,
             20, RED);
    DrawText(TextFormat("Life Steal: %.1f", player->GetLifeSteal()), 120, PADDING + spaceStats * 12, 20, RED);

    // Pintamos la tienda
    DrawRectangle(screenSemiWidth + (screenSemiWidth - widthStats - 100), 100, widthStats, heightStats, WHITE);

    EndDrawing();
}
