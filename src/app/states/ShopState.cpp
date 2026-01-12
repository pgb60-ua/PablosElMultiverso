#include "ShopState.hpp"
#include "Player.hpp"
#include "Shop.hpp"
#include "ShopSlot.hpp"
#include "SpriteLoaderManager.hpp"
#include "SpriteSheet.hpp"
#include "StateMachine.hpp"
#include "Types.hpp"
#include "WeaponFactory.hpp"
#include <cmath>

ShopState::ShopState(Player *player) : player(player), shop() {}
ShopState::~ShopState() {}

// FUNCIONES HELPER DE RENDERIZADO

TWeaponColor ShopState::GetColorBasedOnWeaponLevel(int level)
{
    switch (level)
    {
    case 1:
        return TWeaponColor{
            Color{150, 150, 150, 255}, // Gris
            Color{150, 150, 150, 80},  // Gris transparente
        };
    case 2:
        return TWeaponColor{
            Color{100, 255, 100, 255}, // Verde
            Color{100, 255, 100, 80},  // Verde transparente
        };
    case 3:
        return TWeaponColor{
            Color{200, 100, 255, 255}, // Morado (cambiado de azul)
            Color{200, 100, 255, 80},  // Morado transparente
        };
    case 4:
        return TWeaponColor{
            Color{255, 200, 50, 255}, // Amarillo/Dorado
            Color{255, 200, 50, 80},  // Amarillo transparente
        };
    default:
        return TWeaponColor{
            Color{150, 150, 150, 255}, // Gris por defecto
            Color{150, 150, 150, 80},  // Gris transparente
        };
    }
}

void ShopState::DrawItemSprite(const RenderableItem &item, int x, int y, int maxSize)
{
    Rectangle sourceRec = item.spriteFrame;

    if (item.isWeapon)
    {
        // Calcular escala para que el eje más grande sea maxSize
        float maxAxis = std::max(sourceRec.width, sourceRec.height);
        float scale = (float)maxSize / maxAxis;

        Rectangle destRec = {(float)x, (float)y, sourceRec.width * scale, sourceRec.height * scale};
        DrawTexturePro(item.texture, sourceRec, destRec, Vector2{0, 0}, 0.0f, WHITE);
    }
    else
    {
        // Items normales sin escalar
        DrawTextureRec(item.texture, sourceRec, Vector2{(float)x, (float)y}, WHITE);
    }
}

void ShopState::RenderItemSlot(const RenderableItem &item, Rectangle slotRect, bool showFullInfo)
{
    // Determinar colores del slot según estado
    Color slotBgColor;
    Color borderColor;

    // Color del borde base (por nivel si es arma)
    Color weaponLevelBorderColor = Color{80, 80, 100, 255}; // Default
    if (item.isWeapon)
    {
        TWeaponColor weaponColor = GetColorBasedOnWeaponLevel(item.level);
        weaponLevelBorderColor = weaponColor.borderColor;
    }

    if (item.isSelected)
    {
        slotBgColor = Color{60, 80, 120, 255};
        borderColor = item.isWeapon ? weaponLevelBorderColor : Color{100, 150, 255, 255};
    }
    else if (item.isBlocked)
    {
        slotBgColor = Color{80, 40, 40, 255};
        borderColor = Color{180, 60, 60, 255};
    }
    else
    {
        slotBgColor = Color{45, 45, 65, 255};
        borderColor = weaponLevelBorderColor;
    }

    // Dibujar slot
    DrawRectangle(slotRect.x - 3, slotRect.y - 3, slotRect.width + 6, slotRect.height + 6, borderColor);
    DrawRectangle(slotRect.x, slotRect.y, slotRect.width, slotRect.height, slotBgColor);

    if (showFullInfo)
    {
        // Renderizado completo para items de tienda
        int iconX = slotRect.x + 7;
        int iconY = slotRect.y + 18;
        DrawItemSprite(item, iconX, iconY, 64);

        // Nombre del item
        int textX = slotRect.x + 82;
        DrawText(item.name.c_str(), textX, slotRect.y + 10, 22, Color{255, 255, 255, 255});

        // Descripción del item (truncada si es muy larga)
        if (!item.description.empty())
        {
            std::string description = item.description;
            int maxDescriptionWidth = slotRect.width - 220;
            int descriptionWidth = MeasureText(description.c_str(), 14);

            if (descriptionWidth > maxDescriptionWidth)
            {
                while (descriptionWidth > maxDescriptionWidth && !description.empty())
                {
                    description.pop_back();
                    descriptionWidth = MeasureText((description + "...").c_str(), 14);
                }
                description += "...";
            }

            DrawText(description.c_str(), textX, slotRect.y + 35, 14, Color{180, 180, 200, 255});
        }

        // Rareza con color
        Color rarityColor;
        const char *rarityText;
        switch (item.rarity)
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

        int rarityX = textX;
        DrawText(rarityText, rarityX, slotRect.y + 55, 12, rarityColor);

        // Etiqueta de WEAPON si es un arma
        if (item.isWeapon)
        {
            int weaponTagX = rarityX + MeasureText(rarityText, 12) + 10;
            DrawText("WEAPON", weaponTagX, slotRect.y + 55, 12, Color{255, 200, 100, 255});

            // Mostrar nivel del arma
            int levelTagX = weaponTagX + MeasureText("WEAPON", 12) + 10;
            Color levelColor = GetColorBasedOnWeaponLevel(item.level).borderColor;
            DrawText(TextFormat("Lv%d", item.level), levelTagX, slotRect.y + 55, 12, levelColor);
        }

        // Precio (si existe)
        if (item.price > 0)
        {
            const SpriteSheet &coinSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::COIN);
            int priceX = slotRect.x + slotRect.width - 120;
            Rectangle coinFrame = coinSheet.frames[0];
            int coinYPrice = (slotRect.y + 45) + (20 - (int)coinFrame.height) / 2;
            DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)priceX - 15, (float)coinYPrice}, WHITE);
            DrawText(TextFormat("%d", item.price), priceX + 20, slotRect.y + 45, 20, Color{255, 200, 0, 255});
        }

        // Indicador si está bloqueado
        if (item.isBlocked)
        {
            DrawText("BLOCKED", slotRect.x + 7, slotRect.y + slotRect.height - 20, 14, Color{255, 100, 100, 255});
        }

        // Stats del item
        if (item.stats != nullptr)
        {
            int statsTextX = textX;
            int statsTextY = slotRect.y + 72;
            int statsOffset = 0;

            // Si es arma, multiplicar stats por el nivel: 2^(nivel-1)
            int statsMultiplier = 1;
            if (item.isWeapon)
            {
                statsMultiplier = static_cast<int>(std::pow(2, item.level - 1));
            }

            Color positiveColor = Color{100, 255, 100, 255};
            Color negativeColor = Color{255, 100, 100, 255};

            // Función helper para mostrar stat
            auto drawStat = [&](const char *name, float value)
            {
                if (value != 0)
                {
                    Color color = value > 0 ? positiveColor : negativeColor;
                    const char *sign = value > 0 ? "+" : "";
                    float displayValue = value * statsMultiplier;
                    DrawText(TextFormat("%s%s %.1f", sign, name, displayValue), statsTextX + statsOffset, statsTextY,
                             11, color);
                    statsOffset += MeasureText(TextFormat("%s%s %.1f  ", sign, name, displayValue), 11);
                }
            };

            // Mostrar stats ofensivas
            drawStat("PHY", item.stats->GetPhysicalDamage());
            drawStat("MAG", item.stats->GetMagicDamage());
            drawStat("AS", item.stats->GetAttackSpeed());
            drawStat("CC", item.stats->GetCriticalChance());
            drawStat("CD", item.stats->GetCriticalDamage());
            drawStat("LS", item.stats->GetLifeSteal());

            // Nueva línea para stats defensivas
            statsOffset = 0;
            statsTextY += 13;

            drawStat("HP", item.stats->GetHealth());
            drawStat("MHP", item.stats->GetMaxHealth());
            drawStat("MS", item.stats->GetMovementSpeed());
            drawStat("AGI", item.stats->GetAgility());
            drawStat("ARM", item.stats->GetArmor());
            drawStat("RES", item.stats->GetResistance());
            drawStat("HR", item.stats->GetHealthRegeneration());
        }
    }
    else
    {
        // Renderizado compacto para slots de inventario (cuadrados)
        TWeaponColor weaponColor = GetColorBasedOnWeaponLevel(item.level);

        // Si está seleccionado, borde más grueso
        if (item.isSelected)
        {
            DrawRectangle(slotRect.x - 4, slotRect.y - 4, slotRect.width + 8, slotRect.height + 8,
                          Color{100, 150, 255, 255});
        }

        // Borde del nivel
        DrawRectangle(slotRect.x - 2, slotRect.y - 2, slotRect.width + 4, slotRect.height + 4, weaponColor.borderColor);
        DrawRectangle(slotRect.x, slotRect.y, slotRect.width, slotRect.height, Color{45, 45, 65, 255});

        // Overlay de color del nivel (fondo transparente)
        DrawRectangle(slotRect.x, slotRect.y, slotRect.width, slotRect.height, weaponColor.tintColor);

        // Sprite del item centrado
        Rectangle sourceRec = item.spriteFrame;
        float scale = (slotRect.width - 10) / std::max(sourceRec.width, sourceRec.height);
        Rectangle destRec = {slotRect.x + slotRect.width / 2, slotRect.y + slotRect.height / 2, sourceRec.width * scale,
                             sourceRec.height * scale};
        Vector2 origin = {destRec.width / 2, destRec.height / 2};
        DrawTexturePro(item.texture, sourceRec, destRec, origin, 0.0f, WHITE);

        // Nivel del arma con el color del borde
        const char *levelText = TextFormat("Lv%d", item.level);
        DrawText(levelText, slotRect.x + 5, slotRect.y + slotRect.height - 18, 14, weaponColor.borderColor);
    }
}
void ShopState::handleInput()
{
    // Input de teclado
    if (IsKeyPressed(KEY_S))
    {
        if (selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
        {
            NextSelectedItem(1);
        }
    }
    else if (IsKeyPressed(KEY_W))
    {
        if (selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
        {
            NextSelectedItem(-1);
        }
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        // Si es de tienda
        if (selectedItem >= 0 && selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
        {
            willBuy = true;
        }
        else
        {
            willFuse = true;
        }
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        if (selectedItem >= 0 && selectedItem <= Shop::MAX_ITEMS_SHOP - 1)
        {
            willAlternateBlock = true;
        }
        else
        {
            willSell = true;
        }
    }
    if (IsKeyPressed(KEY_R))
    {
        willReroll = true;
    }
    if (IsKeyPressed(KEY_E))
    {
        passRound = true;
    }
    if (IsKeyPressed(KEY_A))
    {
        NextWeaponSelected(-1);
    }
    if (IsKeyPressed(KEY_D))
    {
        NextWeaponSelected(1);
    }

    // Input de mouse
    Vector2 mousePos = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int headerHeight = 80;

    // Panel de stats del jugador (izquierda)
    int statsX = 40;
    int statsWidth = screenWidth * 0.35f;

    // Panel de items (derecha)
    int itemsX = statsX + statsWidth + 30;
    int itemsY = headerHeight + 20;
    int itemsWidth = screenWidth - itemsX - 40;

    // Dimensiones de slots
    int itemSlotHeight = 100;
    int itemSlotSpacing = 15;
    int itemStartY = itemsY + 70;

    // Detectar hover sobre items de tienda (actualiza selectedItem)
    bool hoveredOverShopItem = false;
    for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
    {
        const TShopSlot &slot = shop.GetItemsShop()[i];
        if (slot.isBuyed)
            continue;

        int slotY = itemStartY + i * (itemSlotHeight + itemSlotSpacing);
        Rectangle slotRect = {(float)(itemsX + 18), (float)slotY, (float)(itemsWidth - 36), (float)itemSlotHeight};

        if (CheckCollisionPointRec(mousePos, slotRect))
        {
            selectedItem = i;
            hoveredOverShopItem = true;
            break;
        }
    }

    // Detectar hover sobre armas del inventario solo si no estamos sobre un item de tienda
    if (!hoveredOverShopItem)
    {
        int weaponSlotSize = 70;
        int weaponSlotSpacing = 10;
        int weaponsPerRow = 5;

        // Calcular posición del panel de armas (mismo cálculo que en render)
        int statSpacing = 25;
        int weaponPanelY = (headerHeight + 20) + 60; // statsY + 60
        int weaponsY = weaponPanelY + 12 * statSpacing + 20;
        int weaponStartX = statsX + 20;
        int weaponStartY = weaponsY + 35;

        const auto &weapons = player->GetWeapons();
        for (int i = 0; i < player->WEAPON_MAX && static_cast<size_t>(i) < weapons.size(); i++)
        {
            int weaponX = weaponStartX + (i % weaponsPerRow) * (weaponSlotSize + weaponSlotSpacing);
            int weaponY = weaponStartY + (i / weaponsPerRow) * (weaponSlotSize + weaponSlotSpacing);

            Rectangle weaponRect = {(float)weaponX, (float)weaponY, (float)weaponSlotSize, (float)weaponSlotSize};

            if (CheckCollisionPointRec(mousePos, weaponRect))
            {
                selectedItem = Shop::MAX_ITEMS_SHOP + i;
                break;
            }
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Verificar clic en items
        for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
        {
            const TShopSlot &slot = shop.GetItemsShop()[i];
            if (slot.isBuyed)
                continue;

            int slotY = itemStartY + i * (itemSlotHeight + itemSlotSpacing);
            Rectangle slotRect = {(float)(itemsX + 18), (float)slotY, (float)(itemsWidth - 36), (float)itemSlotHeight};

            if (CheckCollisionPointRec(mousePos, slotRect))
            {
                willBuy = true;
                break;
            }
        }

        // Verificar clic en botón de reroll
        int rerollButtonWidth = 150;
        int rerollButtonHeight = 40;
        int rerollButtonX = 50;
        int rerollButtonY = 20;
        Rectangle rerollRect = {(float)rerollButtonX, (float)rerollButtonY, (float)rerollButtonWidth,
                                (float)rerollButtonHeight};

        if (CheckCollisionPointRec(mousePos, rerollRect))
        {
            willReroll = true;
        }

        // Verificar clic en botón Continue
        int continueButtonWidth = 150;
        int continueButtonHeight = 40;
        int continueButtonX = rerollButtonX + rerollButtonWidth + 20;
        int continueButtonY = 20;
        Rectangle continueRect = {(float)continueButtonX, (float)continueButtonY, (float)continueButtonWidth,
                                  (float)continueButtonHeight};

        if (CheckCollisionPointRec(mousePos, continueRect))
        {
            passRound = true;
        }
    }

    // Clic derecho para bloquear/desbloquear items
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        Vector2 mousePos = GetMousePosition();
        int screenWidth = GetScreenWidth();
        int headerHeight = 80;

        // Panel de stats del jugador (izquierda)
        int statsX = 40;
        int statsWidth = screenWidth * 0.35f;

        // Panel de items (derecha)
        int itemsX = statsX + statsWidth + 30;
        int itemsY = headerHeight + 20;
        int itemsWidth = screenWidth - itemsX - 40;

        // Dimensiones de slots
        int itemSlotHeight = 100;
        int itemSlotSpacing = 15;
        int itemStartY = itemsY + 70;

        // Verificar clic derecho en items para bloquear
        for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
        {
            const TShopSlot &slot = shop.GetItemsShop()[i];
            if (slot.isBuyed)
                continue;

            int slotY = itemStartY + i * (itemSlotHeight + itemSlotSpacing);
            Rectangle slotRect = {(float)(itemsX + 18), (float)slotY, (float)(itemsWidth - 36), (float)itemSlotHeight};

            if (CheckCollisionPointRec(mousePos, slotRect))
            {
                selectedItem = i;
                willAlternateBlock = true;
                break;
            }
        }
    }

    // Clic izquierdo en los botones FUSE o SELL del tooltip
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && selectedItem >= Shop::MAX_ITEMS_SHOP)
    {
        int weaponIndex = selectedItem - Shop::MAX_ITEMS_SHOP;
        const auto &weapons = player->GetWeapons();

        if (weaponIndex >= 0 && static_cast<size_t>(weaponIndex) < weapons.size())
        {
            // Calcular posición del tooltip y botones (mismo cálculo que en render)
            int statsX = 40;
            int statsWidth = screenWidth * 0.35f;
            int tooltipX = statsX + 20;
            int tooltipWidth = statsWidth - 40;
            int tooltipHeight = 120;

            int statSpacing = 25;
            int weaponPanelY = (headerHeight + 20) + 60;
            int weaponsY = weaponPanelY + 12 * statSpacing + 20;
            int weaponSlotSize = 70;
            int weaponSlotSpacing = 10;
            int weaponStartY = weaponsY + 35;
            int tooltipY = weaponStartY + weaponSlotSize + weaponSlotSpacing + 15;

            // Configuración de botones
            int buttonWidth = 80;
            int buttonHeight = 45;
            int buttonSpacing = 10;
            int totalButtons = 2;
            int buttonsAreaHeight = (buttonHeight * totalButtons) + (buttonSpacing * (totalButtons - 1));
            int buttonsX = tooltipX + tooltipWidth - buttonWidth - 10;

            int statsY = tooltipY + 30;
            int statsAreaHeight = (tooltipY + tooltipHeight - 25) - statsY;
            int buttonsStartY = statsY + (statsAreaHeight - buttonsAreaHeight) / 2;

            // Botón FUSE está arriba
            int button1Y = buttonsStartY;
            Rectangle fuseButtonRect = {(float)buttonsX, (float)button1Y, (float)buttonWidth, (float)buttonHeight};

            if (CheckCollisionPointRec(mousePos, fuseButtonRect))
            {
                willFuse = true;
            }

            // Botón SELL está abajo
            int button2Y = buttonsStartY + buttonHeight + buttonSpacing;
            Rectangle sellButtonRect = {(float)buttonsX, (float)button2Y, (float)buttonWidth, (float)buttonHeight};

            if (CheckCollisionPointRec(mousePos, sellButtonRect))
            {
                willSell = true;
            }
        }
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

        // Calcular precio final (ajustado por nivel si es arma)
        int finalPrice = slot.item->GetPrice();
        bool isWeapon = IsWeaponType(slot.item->GetType());
        if (isWeapon)
        {
            finalPrice *= slot.weaponLevel;
        }

        if (slot.item != nullptr && !slot.isBuyed && player->GetPabloCoins() >= finalPrice)
        {
            // Verificar ANTES de comprar si es un arma y si se puede aceptar
            if (isWeapon)
            {
                WEAPON_TYPE weaponType = ItemTypeToWeaponType(slot.item->GetType());
                int weaponLevel = slot.weaponLevel;
                if (!player->CanAcceptWeapon(weaponType, weaponLevel))
                {
                    // No puede comprar esta arma (todas las armas del mismo tipo estan a mas de level que este)
                    willBuy = false;
                    return;
                }
            }

            const Item *item = shop.BuyItem(selectedItem);
            if (item != nullptr)
            {
                if (isWeapon)
                {
                    player->ModifyPabloCoins(-finalPrice);

                    // Crear el arma usando la factory con el nivel del slot
                    auto weapon =
                        WeaponFactory::CreateWeapon(item->GetType(), player->GetPosition(), player->enemiesInRange,
                                                    player->allEnemies, finalPrice, slot.weaponLevel);
                    if (weapon != nullptr)
                    {
                        player->AddWeapon(std::move(weapon));
                    }
                }
                else
                {
                    player->ModifyPabloCoins(-finalPrice);
                    player->AddItem(item);
                }
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
    if (willFuse)
    {
        if (player->GetWeapons().size() > 1)
        {
            int weaponIndex = selectedItem - Shop::MAX_ITEMS_SHOP;
            if (player->CanFuse(weaponIndex))
            {
                player->UpgradeWeapon(weaponIndex);
            }
        }
        willFuse = false;
    }
    // Vendo si tengo mas de 1 arma
    if (willSell)
    {
        if (player->GetWeapons().size() > 1)
        {

            int weaponIndex = selectedItem - Shop::MAX_ITEMS_SHOP;
            if (weaponIndex >= 0 && static_cast<std::size_t>(weaponIndex) < player->GetWeapons().size())
            {
                int sellPrice = CalculateWeaponSellPrice(weaponIndex);
                player->ModifyPabloCoins(sellPrice);
                player->RemoveWeapon(weaponIndex);
            }
        }
        willSell = false;
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
    const SpriteSheet &coinSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::COIN);
    Rectangle coinFrame = coinSheet.frames[0];

    // Centrar verticalmente la moneda con el texto (25 es el tamaño de la fuente, 30 es la Y del texto)
    int coinYHeader = 30 + (25 - (int)coinFrame.height) / 2;
    DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)coinsX - 20, (float)coinYHeader}, WHITE);
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
    int statSpacing = 25; // Espaciado fijo más compacto

    Color statColor = Color{200, 200, 220, 255};
    Color valueColor = Color{255, 255, 255, 255};

    // Helper para dibujar stat con multiplicador
    auto drawStatWithMultiplier = [&](const char *name, float value, float multiplier)
    {
        Color multColor;
        if (multiplier < 1.0f)
            multColor = Color{255, 100, 100, 255}; // Rojo
        else if (multiplier > 1.0f)
            multColor = Color{100, 255, 100, 255}; // Verde
        else
            multColor = Color{150, 150, 150, 255}; // Gris

        DrawText(TextFormat("x%.2f", multiplier), statsX + 20, statY, 12, multColor);
        DrawText(name, statsX + 75, statY, 14, statColor);
        DrawText(TextFormat("%.1f", value), statsX + statsWidth - 80, statY, 14, valueColor);
        statY += statSpacing;
    };

    drawStatWithMultiplier("Max Health:", player->GetMaxHealth(), player->GetHealthModifier());
    drawStatWithMultiplier("Movement Speed:", player->GetMovementSpeed(), player->GetMovementSpeedModifier());
    drawStatWithMultiplier("Agility:", player->GetAgility(), player->GetAgilityModifier());
    drawStatWithMultiplier("Attack Speed:", player->GetAttackSpeed(), player->GetAttackSpeedModifier());
    drawStatWithMultiplier("Physical Damage:", player->GetPhysicalDamage(), player->GetPhysicalDamageModifier());
    drawStatWithMultiplier("Magical Damage:", player->GetMagicDamage(), player->GetMagicDamageModifier());
    drawStatWithMultiplier("Resistance:", player->GetResistance(), player->GetResistanceModifier());
    drawStatWithMultiplier("Armor:", player->GetArmor(), player->GetArmorModifier());
    drawStatWithMultiplier("Critical Chance:", player->GetCriticalChance(), player->GetCriticalChanceModifier());
    drawStatWithMultiplier("Critical Damage:", player->GetCriticalDamage(), player->GetCriticalDamageModifier());
    drawStatWithMultiplier("Health Regen:", player->GetHealthRegeneration(), player->GetHealthRegenerationModifier());
    drawStatWithMultiplier("Life Steal:", player->GetLifeSteal(), player->GetLifeStealModifier());

    // Panel de armas del jugador (debajo del panel de stats)
    int weaponsY = statY + 20;
    DrawText("WEAPONS", statsX + 20, weaponsY, 20, Color{255, 200, 0, 255});
    DrawLine(statsX + 20, weaponsY + 25, statsX + statsWidth - 20, weaponsY + 25, Color{255, 200, 0, 255});

    int weaponSlotSize = 70;
    int weaponSlotSpacing = 10;
    int weaponStartX = statsX + 20;
    int weaponStartY = weaponsY + 35;
    int weaponsPerRow = 5; // 5 armas por fila

    const auto &weapons = player->GetWeapons();
    for (int i = 0; i < player->WEAPON_MAX; i++)
    {
        int weaponX = weaponStartX + (i % weaponsPerRow) * (weaponSlotSize + weaponSlotSpacing);
        int weaponY = weaponStartY + (i / weaponsPerRow) * (weaponSlotSize + weaponSlotSpacing);

        if (static_cast<size_t>(i) < weapons.size())
        {
            // Dibujar arma existente usando el struct unificado
            const auto &weapon = weapons[i];
            const SpriteSheet &weaponSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(weapon->GetWeaponType());

            if (!weaponSheet.frames.empty())
            {
                RenderableItem item = {
                    weapon->GetType(),                          // itemType
                    weapon->GetName(),                          // name
                    weaponSheet.frames[0],                      // spriteFrame
                    weaponSheet.texture,                        // texture
                    weapon->GetLevel(),                         // level
                    true,                                       // isWeapon
                    (i + Shop::MAX_ITEMS_SHOP == selectedItem), // isSelected
                    false,                                      // isBlocked
                    0,                                          // price
                    nullptr,                                    // stats
                    "",                                         // description
                    ItemRarity::Common                          // rarity
                };

                Rectangle slotRect = {(float)weaponX, (float)weaponY, (float)weaponSlotSize, (float)weaponSlotSize};
                RenderItemSlot(item, slotRect, false); // false = renderizado compacto
            }
        }
        else
        {
            // Dibujar slot vacío
            DrawRectangle(weaponX - 2, weaponY - 2, weaponSlotSize + 4, weaponSlotSize + 4, Color{80, 80, 100, 255});
            DrawRectangle(weaponX, weaponY, weaponSlotSize, weaponSlotSize, Color{60, 60, 70, 255});
            DrawText("EMPTY", weaponX + 10, weaponY + weaponSlotSize / 2 - 7, 12, Color{120, 120, 130, 255});
        }
    }

    // Tooltip de arma seleccionada (si está sobre un arma del inventario)
    if (selectedItem >= Shop::MAX_ITEMS_SHOP)
    {
        int weaponIndex = selectedItem - Shop::MAX_ITEMS_SHOP;
        if (weaponIndex >= 0 && static_cast<size_t>(weaponIndex) < weapons.size())
        {
            const auto &weapon = weapons[weaponIndex];

            // Posición del tooltip (debajo de las armas)
            int tooltipX = statsX + 20;
            int tooltipY = weaponStartY + weaponSlotSize + weaponSlotSpacing + 15;
            int tooltipWidth = statsWidth - 40;
            int tooltipHeight = 120;

            // Fondo del tooltip
            DrawRectangle(tooltipX - 5, tooltipY - 5, tooltipWidth + 10, tooltipHeight + 10, Color{100, 150, 255, 255});
            DrawRectangle(tooltipX, tooltipY, tooltipWidth, tooltipHeight, Color{40, 40, 60, 255});

            // Título
            DrawText(weapon->GetName().c_str(), tooltipX + 10, tooltipY + 5, 16, Color{255, 200, 0, 255});

            // Stats del arma
            int statsY = tooltipY + 30;
            const Stats &weaponStats = weapon->GetStats();

            auto drawWeaponStat = [&](const char *name, float value, int &currentY)
            {
                if (value != 0)
                {
                    Color color = value > 0 ? Color{100, 255, 100, 255} : Color{255, 100, 100, 255};
                    const char *sign = value > 0 ? "+" : "";
                    DrawText(TextFormat("%s%s: %.1f", sign, name, value), tooltipX + 10, currentY, 12, color);
                    currentY += 15;
                }
            };

            // Primera columna
            int col1Y = statsY;
            drawWeaponStat("PHY DMG", weaponStats.GetPhysicalDamage(), col1Y);
            drawWeaponStat("MAG DMG", weaponStats.GetMagicDamage(), col1Y);
            drawWeaponStat("ATK SPD", weaponStats.GetAttackSpeed(), col1Y);

            // Segunda columna (más cerca de la primera)
            int col2Y = statsY;
            int col2X = tooltipX + 120; // Más cerca de la primera columna
            auto drawWeaponStatCol2 = [&](const char *name, float value, int &currentY)
            {
                if (value != 0)
                {
                    Color color = value > 0 ? Color{100, 255, 100, 255} : Color{255, 100, 100, 255};
                    const char *sign = value > 0 ? "+" : "";
                    DrawText(TextFormat("%s%s: %.1f", sign, name, value), col2X, currentY, 12, color);
                    currentY += 15;
                }
            };

            drawWeaponStatCol2("CRIT CH", weaponStats.GetCriticalChance(), col2Y);
            drawWeaponStatCol2("CRIT DMG", weaponStats.GetCriticalDamage(), col2Y);
            drawWeaponStatCol2("LIFE STEAL", weaponStats.GetLifeSteal(), col2Y);

            // Configuración dinámica de botones (verticalmente apilados)
            int buttonWidth = 80;
            int buttonHeight = 45;
            int buttonSpacing = 10;
            int totalButtons = 2; // Número total de botones a mostrar
            int buttonsAreaHeight = (buttonHeight * totalButtons) + (buttonSpacing * (totalButtons - 1));
            int buttonsX = tooltipX + tooltipWidth - buttonWidth - 10;

            // Centrar verticalmente los botones en el área de stats
            // Área disponible: desde statsY hasta el final (antes del precio de venta)
            int statsAreaHeight = (tooltipY + tooltipHeight - 25) - statsY;         // Altura disponible para stats
            int buttonsStartY = statsY + (statsAreaHeight - buttonsAreaHeight) / 2; // Centrado vertical

            // Botón 1: FUSE (arriba)
            int button1Y = buttonsStartY;

            // Verificar si el arma está al nivel máximo
            bool isMaxLevel = weapon->GetLevel() >= weapon->GetMaxLevel();

            // Verificar hover en botón FUSE
            Rectangle fuseButtonRect = {(float)buttonsX, (float)button1Y, (float)buttonWidth, (float)buttonHeight};
            bool isHoveringFuse = CheckCollisionPointRec(GetMousePosition(), fuseButtonRect);

            // Fondo del botón fuse
            Color fuseButtonColor =
                isMaxLevel ? Color{60, 60, 70, 255} : Color{80, 60, 120, 255}; // Gris si max level, morado si no
            Color fuseButtonBorder = isMaxLevel
                                         ? Color{80, 80, 90, 255}
                                         : Color{150, 100, 255, 255}; // Gris si max level, morado brillante si no

            // Borde negro si está en hover
            if (isHoveringFuse)
            {
                fuseButtonBorder = Color{0, 0, 0, 255};
            }

            DrawRectangle(buttonsX - 2, button1Y - 2, buttonWidth + 4, buttonHeight + 4, fuseButtonBorder);
            DrawRectangle(buttonsX, button1Y, buttonWidth, buttonHeight, fuseButtonColor);

            // Texto del botón fuse
            const char *fuseText = isMaxLevel ? "MAX" : "FUSE";
            int fuseTextWidth = MeasureText(fuseText, 14);
            Color fuseTextColor = isMaxLevel ? Color{120, 120, 130, 255} : WHITE;
            DrawText(fuseText, buttonsX + (buttonWidth - fuseTextWidth) / 2, button1Y + 8, 14, fuseTextColor);

            // Si no es el maximo nivel de arma
            if (!isMaxLevel)
            {

                // Leyenda de tecla dentro del botón
                const char *enterText = "[ENTER]";
                int enterTextWidth = MeasureText(enterText, 10);
                DrawText(enterText, buttonsX + (buttonWidth - enterTextWidth) / 2, button1Y + 26, 10,
                         Color{120, 120, 120, 255});
            }

            // Botón 2: SELL (abajo)
            int button2Y = buttonsStartY + buttonHeight + buttonSpacing;

            // Verificar hover en botón SELL
            Rectangle sellButtonRect = {(float)buttonsX, (float)button2Y, (float)buttonWidth, (float)buttonHeight};
            bool isHoveringSell = CheckCollisionPointRec(GetMousePosition(), sellButtonRect);

            // Fondo del botón de vender
            Color sellButtonColor = Color{180, 40, 40, 255};  // Rojo oscuro
            Color sellButtonBorder = Color{255, 80, 80, 255}; // Rojo brillante

            // Borde negro si está en hover
            if (isHoveringSell)
            {
                sellButtonBorder = Color{0, 0, 0, 255};
            }

            DrawRectangle(buttonsX - 2, button2Y - 2, buttonWidth + 4, buttonHeight + 4, sellButtonBorder);
            DrawRectangle(buttonsX, button2Y, buttonWidth, buttonHeight, sellButtonColor);

            // Texto del botón SELL
            const char *sellText = "SELL";
            int sellTextWidth = MeasureText(sellText, 14);
            DrawText(sellText, buttonsX + (buttonWidth - sellTextWidth) / 2, button2Y + 8, 14, WHITE);

            // Leyenda de tecla dentro del botón
            const char *spaceText = "[SPACE]";
            int spaceTextWidth = MeasureText(spaceText, 10);
            DrawText(spaceText, buttonsX + (buttonWidth - spaceTextWidth) / 2, button2Y + 26, 10,
                     Color{200, 200, 200, 255});

            // Precio de venta
            int sellPrice = CalculateWeaponSellPrice(weaponIndex);
            DrawText("Sell Price:", tooltipX + 10, tooltipY + tooltipHeight - 25, 14, Color{200, 200, 220, 255});

            // Dibujar sprite de moneda
            const SpriteSheet &coinSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::COIN);
            int coinTextX = tooltipX + 10 + MeasureText("Sell Price: ", 14);
            Rectangle coinFrame = coinSheet.frames[0];

            // Centrar verticalmente la moneda con el texto (14 es el tamaño de la fuente)
            int textHeight = 14;
            int coinY = (tooltipY + tooltipHeight - 25) + (textHeight - (int)coinFrame.height) / 2;
            DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)coinTextX, (float)coinY}, WHITE);

            // Dibujar cantidad con porcentaje (ajustar el espaciado según el ancho del sprite)
            int priceTextX = coinTextX + (int)coinFrame.width + 5;
            DrawText(TextFormat("%d (50%%)", sellPrice), priceTextX, tooltipY + tooltipHeight - 25, 14,
                     Color{255, 200, 0, 255});
        }
    }

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

        // Determinar si es un arma
        ITEM_TYPE itemType = slot.item->GetType();
        bool isWeapon = (itemType >= ITEM_TYPE::WEAPON_AXE && itemType <= ITEM_TYPE::WEAPON_WING);

        // Calcular precio final (ajustado por nivel si es arma)
        int finalPrice = slot.item->GetPrice();
        if (isWeapon)
        {
            finalPrice *= slot.weaponLevel;
        }

        // Crear item renderizable usando el struct unificado
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(slot.item->GetType());
        const Stats &itemStats = slot.item->GetStats();

        RenderableItem item = {
            itemType,                        // itemType
            slot.item->GetName(),            // name
            sheet.frames[0],                 // spriteFrame
            sheet.texture,                   // texture
            isWeapon ? slot.weaponLevel : 0, // level
            isWeapon,                        // isWeapon
            (i == selectedItem),             // isSelected
            slot.isBlocked,                  // isBlocked
            finalPrice,                      // price
            &itemStats,                      // stats
            slot.item->GetDescription(),     // description
            slot.item->GetItemRarity()       // rarity
        };

        Rectangle slotRect = {(float)(itemsX + 18), (float)slotY, (float)(itemsWidth - 36), (float)itemSlotHeight};
        RenderItemSlot(item, slotRect, true); // true = renderizado completo con toda la info
    }

    // Botón de reroll (posicionado en el header, al lado izquierdo)
    int rerollButtonWidth = 150;
    int rerollButtonHeight = 40;
    int rerollButtonX = 50;
    int rerollButtonY = 20;

    Rectangle rerollButtonRect = {(float)rerollButtonX, (float)rerollButtonY, (float)rerollButtonWidth,
                                  (float)rerollButtonHeight};
    bool isHoveringReroll = CheckCollisionPointRec(GetMousePosition(), rerollButtonRect);

    Color rerollBgColor = player->GetPabloCoins() >= REROLL_COST ? Color{60, 100, 60, 255} : Color{80, 40, 40, 255};
    Color rerollBorderColor =
        player->GetPabloCoins() >= REROLL_COST ? Color{100, 200, 100, 255} : Color{180, 60, 60, 255};

    // Borde negro si está en hover
    if (isHoveringReroll)
    {
        rerollBorderColor = Color{0, 0, 0, 255};
    }

    DrawRectangle(rerollButtonX - 3, rerollButtonY - 3, rerollButtonWidth + 6, rerollButtonHeight + 6,
                  rerollBorderColor);
    DrawRectangle(rerollButtonX, rerollButtonY, rerollButtonWidth, rerollButtonHeight, rerollBgColor);

    const char *rerollText = TextFormat("[R] Reroll $%d", REROLL_COST);
    DrawText(rerollText, rerollButtonX + rerollButtonWidth / 2 - MeasureText(rerollText, 16) / 2, rerollButtonY + 12,
             16, WHITE);

    // Botón de Continue (posicionado en el header, al lado del reroll)
    int continueButtonWidth = 150;
    int continueButtonHeight = 40;
    int continueButtonX = rerollButtonX + rerollButtonWidth + 20;
    int continueButtonY = 20;

    Rectangle continueButtonRect = {(float)continueButtonX, (float)continueButtonY, (float)continueButtonWidth,
                                    (float)continueButtonHeight};
    bool isHoveringContinue = CheckCollisionPointRec(GetMousePosition(), continueButtonRect);

    Color continueBgColor = Color{60, 80, 120, 255};
    Color continueBorderColor = Color{100, 150, 255, 255};

    // Borde negro si está en hover
    if (isHoveringContinue)
    {
        continueBorderColor = Color{0, 0, 0, 255};
    }

    DrawRectangle(continueButtonX - 3, continueButtonY - 3, continueButtonWidth + 6, continueButtonHeight + 6,
                  continueBorderColor);
    DrawRectangle(continueButtonX, continueButtonY, continueButtonWidth, continueButtonHeight, continueBgColor);

    const char *continueText = "[E] Continue";
    DrawText(continueText, continueButtonX + continueButtonWidth / 2 - MeasureText(continueText, 16) / 2,
             continueButtonY + 12, 16, WHITE);

    // Controles en la parte inferior
    int controlsY = screenHeight - 40;
    DrawRectangle(0, controlsY, screenWidth, 40, Color{40, 40, 60, 255});
    DrawRectangle(0, controlsY, screenWidth, 3, Color{255, 200, 0, 255});
    const char *controlsText =
        "[W/S] Navigate  [ENTER/LEFT-CLICK] Buy  [SPACE/RIGHT-CLICK] Lock  [R] Reroll  [E] Continue";
    DrawText(controlsText, screenWidth / 2 - MeasureText(controlsText, 16) / 2, controlsY + 12, 16,
             Color{200, 200, 200, 255});

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

// Navigate weapon selection: direction = -1 for left, 1 for right
void ShopState::NextWeaponSelected(int direction)
{
    int size = player->GetWeapons().size();
    int newSelected = selectedItem + direction;
    // Va para la derecha
    if (direction == 1)
    {
        // 0, 1, 2, 3, 4, 5
        if (newSelected <= Shop::MAX_ITEMS_SHOP)
        {
            selectedItem = 5;
        }
        // 6, 7, 8
        else if (newSelected < Shop::MAX_ITEMS_SHOP + size)
        {
            selectedItem = newSelected;
        }
        // 9 ... etc
        else
        {
            selectedItem = -1;
            NextSelectedItem(1);
        }
    }
    // Va para la izquierda
    else if (direction == -1)
    {
        // etc .... 0, 1, 2, 3
        if (newSelected < Shop::MAX_ITEMS_SHOP - 1)
        {
            selectedItem = Shop::MAX_ITEMS_SHOP + size - 1;
        }
        // 4
        else if (newSelected == Shop::MAX_ITEMS_SHOP - 1)
        {
            selectedItem = -1;
            NextSelectedItem(1);
        }
        else
        {
            selectedItem = newSelected;
        }
    }
}

int ShopState::CalculateWeaponSellPrice(int weaponIndex)
{
    const auto &weapons = player->GetWeapons();
    if (weaponIndex < 0 || static_cast<std::size_t>(weaponIndex) >= weapons.size())
    {
        return 0;
    }

    const auto &weapon = weapons[weaponIndex];
    int price = weapon->GetPrice(); // Precio total del arma
    return price / 2;               // Te devuelve la mitad de lo que te ha costado comprarla en total
}
