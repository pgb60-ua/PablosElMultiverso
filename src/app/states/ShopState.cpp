#include "ShopState.hpp"
#include "Player.hpp"
#include "ShopSlot.hpp"
#include "StateMachine.hpp"
#include "raylib.h"

ShopState::ShopState(Player *player) : player(player), shop() {}
ShopState::~ShopState() {}
void ShopState::init() { player->ModifyPabloCoins(50); }
void ShopState::handleInput()
{
    if (IsKeyPressed(KEY_S))
    {
        NextSelectedItem(1);
    }
    else if (IsKeyPressed(KEY_W))
    {
        NextSelectedItem(-1);
    }
    // Quiero que tenga boton para avanzar ronda
    if (IsKeyPressed(KEY_ENTER) && selectedItem >= 0 && selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
    {
        willBuy = true;
    }
    if (IsKeyPressed(KEY_SPACE) && selectedItem > 0 && selectedItem < Shop::MAX_ITEMS_SHOP - 1)
    {
        willAlternateBlock = true;
    }
    if (IsKeyPressed(KEY_E))
    {
        passRound = true;
    }
}
void ShopState::update(float deltaTime)
{
    if (passRound)
    {
        passRound = false;
        state_machine->remove_state(false);
    }
    if (willAlternateBlock)
    {
        shop.AlternateBlockSlot(selectedItem);
        willAlternateBlock = false;
    }
    if (willBuy)
    {
        const TShopSlot &slot = shop.GetItemsShop()[selectedItem];
        if (slot.item != nullptr && !slot.isBuyed && player->GetPabloCoins() >= slot.item->GetPrice())
        {
            const Item *item = shop.BuyItem(selectedItem);
            if (item != nullptr)
            {
                player->ModifyPabloCoins(-item->GetPrice());
                player->AddItem(item);
            }
        }
        willBuy = false;
    }
    // if rerroll
}
void ShopState::render()
{
    int screenSemiWidth = GetScreenWidth() / 2;
    int heightStats = GetScreenHeight() - 200;
    int widthStats = screenSemiWidth - GetScreenWidth() / 6;
    int minXShop = screenSemiWidth + (screenSemiWidth - widthStats - 100);

    // Entre 13 porque muestro 12 stats y dejo 1 para margen de arriba y abajo
    int spaceStats = heightStats / 13;

    BeginDrawing();

    // ClearBackground(BLACK);

    // Dibujamos cuadrado de menu de tienda
    DrawRectangle(50, 50, GetScreenWidth() - 100, GetScreenHeight() - 100, BLACK);

    // Pintamos las pablo coins (le doy 110 para margen)
    DrawText(TextFormat("Pablo Coins: %d", player->GetPabloCoins()), widthStats + 110, 100, 20, WHITE);

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

    // Pintamos la tienda (los items)
    DrawRectangle(minXShop, 100, widthStats, heightStats, WHITE);
    // Pintamos los items
    for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
    {
        Color color = GRAY;
        const TShopSlot &slot = shop.GetItemsShop()[i];
        // Si el item esta comprado no aparece
        if (slot.isBuyed)
        {
            continue;
        }
        if (i == selectedItem)
        {
            color = BLUE;
        }
        else if (slot.isBlocked)
        {
            color = RED;
        }

        DrawRectangle(minXShop, 100 + i * (heightStats / 5), widthStats, 64, color);
        DrawText(slot.item->GetName().c_str(), minXShop + 70, 100 + i * (heightStats / 5), 20, WHITE);

        // DrawTexture(*slot.item->GetIcon(), minXShop, 100 + i * (heightStats / 5), WHITE);
    }

    EndDrawing();
}

void ShopState::NextSelectedItem(int direction)
{
    int newSelection = selectedItem + direction;

    // Buscar el siguiente item válido en la dirección indicada
    while (newSelection >= 0 && newSelection < Shop::MAX_ITEMS_SHOP)
    {
        // Si encontramos un item válido (no comprado/no null)
        if (!shop.GetItemsShop()[newSelection].isBuyed)
        {
            selectedItem = newSelection;
            return;
        }

        newSelection += direction;
    }

    // Si llegamos aquí, no hay items válidos en esa dirección, no hacemos nada
}
