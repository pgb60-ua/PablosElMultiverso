#include "ShopState.hpp"
#include "Player.hpp"
#include "ShopSlot.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
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
    if (IsKeyPressed(KEY_ENTER) && selectedItem >= 0 && selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
    {
        willBuy = true;
    }
    if (IsKeyPressed(KEY_SPACE) && selectedItem >= 0 && selectedItem < Shop::MAX_ITEMS_SHOP)
    {
        willAlternateBlock = true;
    }
    if (IsKeyPressed(KEY_R))
    {
        willReroll = true;
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
    if (willReroll)
    {
        if (player->GetPabloCoins() >= REROLL_COST)
        {
            shop.reRoll();
            player->ModifyPabloCoins(-REROLL_COST);
            selectedItem = 0;
        }
        willReroll = false;
    }
}
void ShopState::render()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    BeginDrawing();

    // Fondo con gradiente simulado
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{20, 20, 30, 255});

    // Header de la tienda
    int headerHeight = 80;
    DrawRectangle(0, 0, screenWidth, headerHeight, Color{40, 40, 60, 255});
    DrawRectangle(0, headerHeight - 3, screenWidth, 3, Color{255, 200, 0, 255});

    DrawText("SHOP", screenWidth / 2 - MeasureText("SHOP", 40) / 2, 20, 40, Color{255, 200, 0, 255});

    // Pablo Coins con icono
    int coinsX = screenWidth - 250;
    DrawCircle(coinsX, 40, 20, Color{255, 200, 0, 255});
    DrawText("$", coinsX - 7, 28, 25, BLACK);
    DrawText(TextFormat("%d", player->GetPabloCoins()), coinsX + 30, 30, 25, Color{255, 255, 255, 255});

    // Panel de stats del jugador (izquierda)
    int statsX = 40;
    int statsY = headerHeight + 20;
    int statsWidth = screenWidth * 0.35f;
    int statsHeight = screenHeight - headerHeight - 60;

    // Fondo del panel de stats
    DrawRectangle(statsX - 5, statsY - 5, statsWidth + 10, statsHeight + 10, Color{255, 200, 0, 255});
    DrawRectangle(statsX, statsY, statsWidth, statsHeight, Color{30, 30, 45, 255});

    // Título del panel
    DrawText("PLAYER STATS", statsX + 20, statsY + 15, 24, Color{255, 200, 0, 255});
    DrawLine(statsX + 20, statsY + 45, statsX + statsWidth - 20, statsY + 45, Color{255, 200, 0, 255});

    // Stats con mejor formato
    int statY = statsY + 60;
    int statSpacing = (statsHeight - 80) / 12;

    Color statColor = Color{200, 200, 220, 255};
    Color valueColor = Color{255, 255, 255, 255};

    DrawText("Max Health:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetMaxHealth()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Movement Speed:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetMovementSpeed()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Agility:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetAgility()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Attack Speed:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetAttackSpeed()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Physical Damage:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetPhysicalDamage()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Magical Damage:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetMagicDamage()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Resistance:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetResistance()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Armor:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetArmor()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Critical Chance:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetCriticalChance()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Critical Damage:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetCriticalDamage()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Health Regen:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetHealthRegeneration()), statsX + statsWidth - 80, statY, 18, valueColor);
    statY += statSpacing;

    DrawText("Life Steal:", statsX + 20, statY, 18, statColor);
    DrawText(TextFormat("%.1f", player->GetLifeSteal()), statsX + statsWidth - 80, statY, 18, valueColor);

    // Panel de items (derecha)
    int itemsX = statsX + statsWidth + 30;
    int itemsY = headerHeight + 20;
    int itemsWidth = screenWidth - itemsX - 40;
    int itemsHeight = statsHeight;

    // Fondo del panel de items
    DrawRectangle(itemsX - 5, itemsY - 5, itemsWidth + 10, itemsHeight + 10, Color{255, 200, 0, 255});
    DrawRectangle(itemsX, itemsY, itemsWidth, itemsHeight, Color{30, 30, 45, 255});

    // Título del panel
    DrawText("AVAILABLE ITEMS", itemsX + 20, itemsY + 15, 24, Color{255, 200, 0, 255});
    DrawLine(itemsX + 20, itemsY + 45, itemsX + itemsWidth - 20, itemsY + 45, Color{255, 200, 0, 255});

    // Items
    int itemSlotHeight = 100;
    int itemSlotSpacing = 15;
    int itemStartY = itemsY + 70;

    for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
    {
        const TShopSlot &slot = shop.GetItemsShop()[i];

        if (slot.isBuyed)
            continue;

        int slotY = itemStartY + i * (itemSlotHeight + itemSlotSpacing);

        // Color del slot según estado
        Color slotBgColor;
        Color borderColor;

        if (i == selectedItem)
        {
            slotBgColor = Color{60, 80, 120, 255};
            borderColor = Color{100, 150, 255, 255};
        }
        else if (slot.isBlocked)
        {
            slotBgColor = Color{80, 40, 40, 255};
            borderColor = Color{180, 60, 60, 255};
        }
        else
        {
            slotBgColor = Color{45, 45, 65, 255};
            borderColor = Color{80, 80, 100, 255};
        }

        // Borde del slot
        DrawRectangle(itemsX + 15, slotY - 3, itemsWidth - 30, itemSlotHeight + 6, borderColor);
        DrawRectangle(itemsX + 18, slotY, itemsWidth - 36, itemSlotHeight, slotBgColor);

        // Icono del item
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(slot.item->GetType());
        DrawTexture(sheet.texture, itemsX + 25, slotY + 18, WHITE);

        // Nombre del item
        int textX = itemsX + 100;
        DrawText(slot.item->GetName().c_str(), textX, slotY + 10, 22, Color{255, 255, 255, 255});

        // Descripción del item (primera línea)
        DrawText(slot.item->GetDescription().c_str(), textX, slotY + 35, 14, Color{180, 180, 200, 255});

        // Rareza con color
        Color rarityColor;
        const char *rarityText;
        switch (slot.item->GetItemRarity())
        {
        case ItemRarity::Common:
            rarityColor = Color{150, 150, 150, 255};
            rarityText = "COMMON";
            break;
        case ItemRarity::Uncommon:
            rarityColor = Color{100, 255, 100, 255};
            rarityText = "UNCOMMON";
            break;
        case ItemRarity::Rare:
            rarityColor = Color{100, 150, 255, 255};
            rarityText = "RARE";
            break;
        case ItemRarity::Epic:
            rarityColor = Color{200, 100, 255, 255};
            rarityText = "EPIC";
            break;
        case ItemRarity::Legendary:
            rarityColor = Color{255, 180, 50, 255};
            rarityText = "LEGENDARY";
            break;
        default:
            rarityColor = WHITE;
            rarityText = "UNKNOWN";
            break;
        }
        DrawText(rarityText, textX, slotY + 55, 12, rarityColor);

        // Precio
        int priceX = itemsX + itemsWidth - 120;
        DrawCircle(priceX, slotY + 50, 15, Color{255, 200, 0, 255});
        DrawText("$", priceX - 5, slotY + 42, 18, BLACK);
        DrawText(TextFormat("%d", slot.item->GetPrice()), priceX + 20, slotY + 45, 20, Color{255, 200, 0, 255});

        // Indicador si está bloqueado
        if (slot.isBlocked)
        {
            DrawText("BLOCKED", itemsX + 25, slotY + itemSlotHeight - 20, 14, Color{255, 100, 100, 255});
        }

        // Stats del item (mostrar solo si está seleccionado)
        if (i == selectedItem)
        {
            int statsTextX = textX;
            int statsTextY = slotY + 72;
            int statsOffset = 0;

            const Stats &itemStats = slot.item->GetStats();
            Color positiveColor = Color{100, 255, 100, 255};
            Color negativeColor = Color{255, 100, 100, 255};

            // Función helper para mostrar stat
            auto drawStat = [&](const char *name, float value)
            {
                if (value != 0)
                {
                    Color color = value > 0 ? positiveColor : negativeColor;
                    const char *sign = value > 0 ? "+" : "";
                    DrawText(TextFormat("%s%s %.1f", sign, name, value), statsTextX + statsOffset, statsTextY, 11,
                             color);
                    statsOffset += MeasureText(TextFormat("%s%s %.1f  ", sign, name, value), 11);
                }
            };

            // Mostrar stats ofensivas
            drawStat("PHY", itemStats.GetPhysicalDamage());
            drawStat("MAG", itemStats.GetMagicDamage());
            drawStat("AS", itemStats.GetAttackSpeed());
            drawStat("CC", itemStats.GetCriticalChance());
            drawStat("CD", itemStats.GetCriticalDamage());
            drawStat("LS", itemStats.GetLifeSteal());

            // Nueva línea para stats defensivas
            statsOffset = 0;
            statsTextY += 13;

            drawStat("HP", itemStats.GetHealth());
            drawStat("MHP", itemStats.GetMaxHealth());
            drawStat("MS", itemStats.GetMovementSpeed());
            drawStat("AGI", itemStats.GetAgility());
            drawStat("ARM", itemStats.GetArmor());
            drawStat("RES", itemStats.GetResistance());
            drawStat("HR", itemStats.GetHealthRegeneration());
        }
    }

    // Botón de reroll (posicionado en el header, al lado izquierdo)
    int rerollButtonWidth = 150;
    int rerollButtonHeight = 40;
    int rerollButtonX = 50;
    int rerollButtonY = 20;

    Color rerollBgColor = player->GetPabloCoins() >= REROLL_COST ? Color{60, 100, 60, 255} : Color{80, 40, 40, 255};
    Color rerollBorderColor =
        player->GetPabloCoins() >= REROLL_COST ? Color{100, 200, 100, 255} : Color{180, 60, 60, 255};

    DrawRectangle(rerollButtonX - 3, rerollButtonY - 3, rerollButtonWidth + 6, rerollButtonHeight + 6,
                  rerollBorderColor);
    DrawRectangle(rerollButtonX, rerollButtonY, rerollButtonWidth, rerollButtonHeight, rerollBgColor);

    const char *rerollText = TextFormat("[R] Reroll $%d", REROLL_COST);
    DrawText(rerollText, rerollButtonX + rerollButtonWidth / 2 - MeasureText(rerollText, 16) / 2, rerollButtonY + 12,
             16, WHITE);

    // Controles en la parte inferior
    int controlsY = screenHeight - 40;
    DrawRectangle(0, controlsY, screenWidth, 40, Color{40, 40, 60, 255});
    DrawRectangle(0, controlsY, screenWidth, 3, Color{255, 200, 0, 255});
    DrawText("[W/S] Navigate  [ENTER] Buy  [SPACE] Lock/Unlock  [R] Reroll  [E] Continue",
             screenWidth / 2 -
                 MeasureText("[W/S] Navigate  [ENTER] Buy  [SPACE] Lock/Unlock  [R] Reroll  [E] Continue", 16) / 2,
             controlsY + 12, 16, Color{200, 200, 200, 255});

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
