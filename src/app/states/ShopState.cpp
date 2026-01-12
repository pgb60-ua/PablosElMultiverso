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

ShopLayout ShopState::CalculateLayout()
{
    ShopLayout layout;

    layout.screenWidth = GetScreenWidth();
    layout.screenHeight = GetScreenHeight();
    layout.headerHeight = 80;
    layout.statsX = 40;
    layout.statsY = layout.headerHeight + 20;
    layout.statsWidth = layout.screenWidth * 0.35f;
    layout.statsHeight = layout.screenHeight - layout.headerHeight - 60;
    layout.itemsX = layout.statsX + layout.statsWidth + 30;
    layout.itemsY = layout.headerHeight + 20;
    layout.itemsWidth = layout.screenWidth - layout.itemsX - 40;
    layout.itemsHeight = layout.statsHeight;
    layout.itemSlotHeight = 100;
    layout.itemSlotSpacing = 15;
    layout.itemStartY = layout.itemsY + 70;
    layout.weaponSlotSize = 70;
    layout.weaponSlotSpacing = 10;
    layout.weaponsPerRow = 5;
    layout.statSpacing = 25;
    layout.weaponPanelY = layout.statsY + 60;
    layout.weaponsY = layout.weaponPanelY + 12 * layout.statSpacing + 20;
    layout.weaponStartX = layout.statsX + 20;
    layout.weaponStartY = layout.weaponsY + 35;

    return layout;
}

TWeaponColor ShopState::GetColorBasedOnWeaponLevel(int level)
{
    switch (level)
    {
    case 1:
        return TWeaponColor{
            WEAPON_LEVEL1_BORDER, // Gris
            WEAPON_LEVEL1_TINT,   // Gris transparente
        };
    case 2:
        return TWeaponColor{
            WEAPON_LEVEL2_BORDER, // Verde
            WEAPON_LEVEL2_TINT,   // Verde transparente
        };
    case 3:
        return TWeaponColor{
            WEAPON_LEVEL3_BORDER, // Morado (cambiado de azul)
            WEAPON_LEVEL3_TINT,   // Morado transparente
        };
    case 4:
        return TWeaponColor{
            WEAPON_LEVEL4_BORDER, // Amarillo/Dorado
            WEAPON_LEVEL4_TINT,   // Amarillo transparente
        };
    default:
        return TWeaponColor{
            WEAPON_LEVEL1_BORDER, // Gris por defecto
            WEAPON_LEVEL1_TINT,   // Gris transparente
        };
    }
}

void ShopState::DrawItemSprite(const RenderableItem &item, int x, int y, int maxSize)
{
    const SpriteSheet *sheetPtr = nullptr;

    if (item.isWeapon)
    {
        // Para armas, convertir ITEM_TYPE a WEAPON_TYPE
        WEAPON_TYPE weaponType = ItemTypeToWeaponType(item.itemType);
        sheetPtr = &SpriteLoaderManager::GetInstance().GetSpriteSheet(weaponType);
    }
    else
    {
        // Para items normales, usar ITEM_TYPE directamente
        sheetPtr = &SpriteLoaderManager::GetInstance().GetSpriteSheet(item.itemType);
    }

    if (sheetPtr->frames.empty())
        return;

    Rectangle sourceRec = sheetPtr->frames[0];

    if (item.isWeapon)
    {
        // Calcular escala para que el eje más grande sea maxSize
        float maxAxis = std::max(sourceRec.width, sourceRec.height);
        float scale = (float)maxSize / maxAxis;

        Rectangle destRec = {(float)x, (float)y, sourceRec.width * scale, sourceRec.height * scale};
        DrawTexturePro(sheetPtr->texture, sourceRec, destRec, Vector2{0, 0}, 0.0f, WHITE);
    }
    else
    {
        // Items normales sin escalar
        DrawTextureRec(sheetPtr->texture, sourceRec, Vector2{(float)x, (float)y}, WHITE);
    }
}

void ShopState::DrawButton(Rectangle rect, const char *text, bool isEnabled, Color enabledBg, Color disabledBg,
                           Color enabledBorder, Color disabledBorder, const char *keyHint, int price)
{
    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, rect);

    Color bgColor = isEnabled ? enabledBg : disabledBg;
    Color borderColor = isEnabled ? enabledBorder : disabledBorder;

    // Borde negro si está en hover y está habilitado
    if (isHovering && isEnabled)
    {
        borderColor = BORDER_HOVER;
    }

    // Dibujar borde y fondo
    DrawRectangle(rect.x - 3, rect.y - 3, rect.width + 6, rect.height + 6, borderColor);
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, bgColor);

    Color textColor = isEnabled ? WHITE : TEXT_DISABLED;
    int textY = keyHint != nullptr ? rect.y + 8 : rect.y + (rect.height - 16) / 2;

    // Si hay precio, dibujar texto + sprite + precio en la misma línea
    if (price >= 0)
    {
        const SpriteSheet &coinSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::COIN);
        Rectangle coinFrame = coinSheet.frames[0];
        const char *priceText = TextFormat("%d", price);
        int textWidth = MeasureText(text, 16);
        int priceTextWidth = MeasureText(priceText, 16);

        // Calcular ancho total (texto + espacio + moneda + espacio + precio)
        int totalWidth = textWidth + 5 + (int)coinFrame.width + 3 + priceTextWidth;
        int startX = rect.x + (rect.width - totalWidth) / 2;

        // Dibujar texto principal
        DrawText(text, startX, textY, 16, textColor);

        // Dibujar sprite de moneda (centrado verticalmente con el texto)
        int coinX = startX + textWidth + 5;
        int coinY = textY + (16 - (int)coinFrame.height) / 2;
        DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)coinX, (float)coinY}, WHITE);

        // Dibujar precio
        int priceX = coinX + (int)coinFrame.width + 3;
        DrawText(priceText, priceX, textY, 16, TEXT_ACCENT);
    }
    else
    {
        // Sin precio, solo centrar el texto
        int textWidth = MeasureText(text, 16);
        DrawText(text, rect.x + (rect.width - textWidth) / 2, textY, 16, textColor);
    }

    // Dibujar hint de tecla si existe
    if (keyHint != nullptr && isEnabled)
    {
        int hintWidth = MeasureText(keyHint, 10);
        DrawText(keyHint, rect.x + (rect.width - hintWidth) / 2, rect.y + 26, 10, TEXT_HINT);
    }
}

void ShopState::DrawStatWithMultiplier(const char *name, float value, float multiplier, int &y, int x, int spacing,
                                       int statsWidth)
{
    Color multColor;
    if (multiplier < 1.0f)
        multColor = MULTIPLIER_NEGATIVE; // Rojo
    else if (multiplier > 1.0f)
        multColor = MULTIPLIER_POSITIVE; // Verde
    else
        multColor = MULTIPLIER_NEUTRAL; // Gris

    Color statColor = TEXT_SECONDARY;
    Color valueColor = TEXT_PRIMARY;

    DrawText(TextFormat("x%.2f", multiplier), x + 20, y, 12, multColor);
    DrawText(name, x + 75, y, 14, statColor);
    DrawText(TextFormat("%.1f", value), x + statsWidth - 80, y, 14, valueColor);
    y += spacing;
}

void ShopState::RenderItemSlot(const RenderableItem &item, Rectangle slotRect, bool showFullInfo)
{
    // Determinar colores del slot según estado
    Color slotBgColor;
    Color borderColor;

    // Color del borde base (por nivel si es arma)
    Color weaponLevelBorderColor = BORDER_DEFAULT; // Default
    if (item.isWeapon)
    {
        TWeaponColor weaponColor = GetColorBasedOnWeaponLevel(item.level);
        weaponLevelBorderColor = weaponColor.borderColor;
    }

    if (item.isSelected)
    {
        slotBgColor = BG_SLOT_SELECTED;
        borderColor = item.isWeapon ? weaponLevelBorderColor : BORDER_SELECTED;
    }
    else if (item.isBlocked)
    {
        slotBgColor = BG_SLOT_BLOCKED;
        borderColor = BORDER_BLOCKED;
    }
    else
    {
        slotBgColor = BG_SLOT;
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
        DrawText(item.name.c_str(), textX, slotRect.y + 10, 22, TEXT_PRIMARY);

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

            DrawText(description.c_str(), textX, slotRect.y + 35, 14, TEXT_TERTIARY);
        }

        // Rareza con color
        Color rarityColor;
        const char *rarityText;
        switch (item.rarity)
        {
        case ItemRarity::Common:
            rarityColor = RARITY_COMMON;
            rarityText = "COMMON";
            break;
        case ItemRarity::Uncommon:
            rarityColor = RARITY_UNCOMMON;
            rarityText = "UNCOMMON";
            break;
        case ItemRarity::Rare:
            rarityColor = RARITY_RARE;
            rarityText = "RARE";
            break;
        case ItemRarity::Epic:
            rarityColor = RARITY_EPIC;
            rarityText = "EPIC";
            break;
        case ItemRarity::Legendary:
            rarityColor = RARITY_LEGENDARY;
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
            DrawText("WEAPON", weaponTagX, slotRect.y + 55, 12, WEAPON_TAG_COLOR);

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
            DrawText(TextFormat("%d", item.price), priceX + 20, slotRect.y + 45, 20, TEXT_ACCENT);
        }

        // Indicador si está bloqueado
        if (item.isBlocked)
        {
            DrawText("BLOCKED", slotRect.x + 7, slotRect.y + slotRect.height - 20, 14, TEXT_NEGATIVE);
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

            Color positiveColor = TEXT_POSITIVE;
            Color negativeColor = TEXT_NEGATIVE;

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
            DrawRectangle(slotRect.x - 4, slotRect.y - 4, slotRect.width + 8, slotRect.height + 8, BORDER_SELECTED);
        }

        // Borde del nivel
        DrawRectangle(slotRect.x - 2, slotRect.y - 2, slotRect.width + 4, slotRect.height + 4, weaponColor.borderColor);
        DrawRectangle(slotRect.x, slotRect.y, slotRect.width, slotRect.height, BG_SLOT);

        // Overlay de color del nivel (fondo transparente)
        DrawRectangle(slotRect.x, slotRect.y, slotRect.width, slotRect.height, weaponColor.tintColor);

        // Sprite del item centrado
        const SpriteSheet *sheetPtr = nullptr;
        if (item.isWeapon)
        {
            WEAPON_TYPE weaponType = ItemTypeToWeaponType(item.itemType);
            sheetPtr = &SpriteLoaderManager::GetInstance().GetSpriteSheet(weaponType);
        }
        else
        {
            sheetPtr = &SpriteLoaderManager::GetInstance().GetSpriteSheet(item.itemType);
        }

        if (!sheetPtr->frames.empty())
        {
            Rectangle sourceRec = sheetPtr->frames[0];
            float scale = (slotRect.width - 10) / std::max(sourceRec.width, sourceRec.height);
            Rectangle destRec = {slotRect.x + slotRect.width / 2, slotRect.y + slotRect.height / 2,
                                 sourceRec.width * scale, sourceRec.height * scale};
            Vector2 origin = {destRec.width / 2, destRec.height / 2};
            DrawTexturePro(sheetPtr->texture, sourceRec, destRec, origin, 0.0f, WHITE);
        }

        // Nivel del arma con el color del borde
        const char *levelText = TextFormat("Lv%d", item.level);
        DrawText(levelText, slotRect.x + 5, slotRect.y + slotRect.height - 18, 14, weaponColor.borderColor);
    }
}

// END FUNCIONES HELPER DE RENDERIZADO

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
    ShopLayout layout = CalculateLayout();

    // Detectar hover sobre items de tienda (actualiza selectedItem)
    bool hoveredOverShopItem = false;
    for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
    {
        const TShopSlot &slot = shop.GetItemsShop()[i];
        if (slot.isBuyed)
            continue;

        int slotY = layout.itemStartY + i * (layout.itemSlotHeight + layout.itemSlotSpacing);
        Rectangle slotRect = {(float)(layout.itemsX + 18), (float)slotY, (float)(layout.itemsWidth - 36),
                              (float)layout.itemSlotHeight};

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
        const auto &weapons = player->GetWeapons();
        for (int i = 0; i < player->WEAPON_MAX && static_cast<size_t>(i) < weapons.size(); i++)
        {
            int weaponX =
                layout.weaponStartX + (i % layout.weaponsPerRow) * (layout.weaponSlotSize + layout.weaponSlotSpacing);
            int weaponY =
                layout.weaponStartY + (i / layout.weaponsPerRow) * (layout.weaponSlotSize + layout.weaponSlotSpacing);

            Rectangle weaponRect = {(float)weaponX, (float)weaponY, (float)layout.weaponSlotSize,
                                    (float)layout.weaponSlotSize};

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

            int slotY = layout.itemStartY + i * (layout.itemSlotHeight + layout.itemSlotSpacing);
            Rectangle slotRect = {(float)(layout.itemsX + 18), (float)slotY, (float)(layout.itemsWidth - 36),
                                  (float)layout.itemSlotHeight};

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
        // Verificar clic derecho en items para bloquear
        for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
        {
            const TShopSlot &slot = shop.GetItemsShop()[i];
            if (slot.isBuyed)
                continue;

            int slotY = layout.itemStartY + i * (layout.itemSlotHeight + layout.itemSlotSpacing);
            Rectangle slotRect = {(float)(layout.itemsX + 18), (float)slotY, (float)(layout.itemsWidth - 36),
                                  (float)layout.itemSlotHeight};

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
            // Calcular posición del tooltip y botones
            int tooltipX = layout.statsX + 20;
            int tooltipWidth = layout.statsWidth - 40;
            int tooltipHeight = 120;
            int tooltipY = layout.weaponStartY + layout.weaponSlotSize + layout.weaponSlotSpacing + 15;

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
        player->SetHealth(player->GetMaxHealth());
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
    // Constantes de layout calculadas una sola vez
    ShopLayout layout = CalculateLayout();

    // Obtener sprite de moneda una sola vez
    const SpriteSheet &coinSheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ITEM_TYPE::COIN);
    Rectangle coinFrame = coinSheet.frames[0];

    BeginDrawing();

    // Fondo con gradiente simulado
    DrawRectangle(0, 0, layout.screenWidth, layout.screenHeight, BG_MAIN);

    // Header de la tienda
    DrawRectangle(0, 0, layout.screenWidth, layout.headerHeight, BG_HEADER);
    DrawRectangle(0, layout.headerHeight - 3, layout.screenWidth, 3, BORDER_ACCENT);

    DrawText("SHOP", layout.screenWidth / 2 - MeasureText("SHOP", 40) / 2, 20, 40, TEXT_ACCENT);

    // Pablo Coins con icono
    int coinsX = layout.screenWidth - 250;
    int coinYHeader = 30 + (25 - (int)coinFrame.height) / 2;
    DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)coinsX - 20, (float)coinYHeader}, WHITE);
    DrawText(TextFormat("%d", player->GetPabloCoins()), coinsX + 30, 30, 25, TEXT_PRIMARY);

    // Fondo del panel de stats
    DrawRectangle(layout.statsX - 5, layout.statsY - 5, layout.statsWidth + 10, layout.statsHeight + 10, BORDER_ACCENT);
    DrawRectangle(layout.statsX, layout.statsY, layout.statsWidth, layout.statsHeight, BG_PANEL);

    // Título del panel
    DrawText("PLAYER STATS", layout.statsX + 20, layout.statsY + 15, 24, TEXT_ACCENT);
    DrawLine(layout.statsX + 20, layout.statsY + 45, layout.statsX + layout.statsWidth - 20, layout.statsY + 45,
             BORDER_ACCENT);

    // Stats con mejor formato
    int statY = layout.statsY + 60;

    DrawStatWithMultiplier("Max Health:", player->GetMaxHealth(), player->GetHealthModifier(), statY, layout.statsX,
                           layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Movement Speed:", player->GetMovementSpeed(), player->GetMovementSpeedModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Agility:", player->GetAgility(), player->GetAgilityModifier(), statY, layout.statsX,
                           layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Attack Speed:", player->GetAttackSpeed(), player->GetAttackSpeedModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Physical Damage:", player->GetPhysicalDamage(), player->GetPhysicalDamageModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Magical Damage:", player->GetMagicDamage(), player->GetMagicDamageModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Resistance:", player->GetResistance(), player->GetResistanceModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Armor:", player->GetArmor(), player->GetArmorModifier(), statY, layout.statsX,
                           layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Critical Chance:", player->GetCriticalChance(), player->GetCriticalChanceModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Critical Damage:", player->GetCriticalDamage(), player->GetCriticalDamageModifier(), statY,
                           layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Health Regen:", player->GetHealthRegeneration(), player->GetHealthRegenerationModifier(),
                           statY, layout.statsX, layout.statSpacing, layout.statsWidth);
    DrawStatWithMultiplier("Life Steal:", player->GetLifeSteal(), player->GetLifeStealModifier(), statY, layout.statsX,
                           layout.statSpacing, layout.statsWidth);

    // Panel de armas del jugador (debajo del panel de stats)
    DrawText("WEAPONS", layout.statsX + 20, layout.weaponsY, 20, TEXT_ACCENT);
    DrawLine(layout.statsX + 20, layout.weaponsY + 25, layout.statsX + layout.statsWidth - 20, layout.weaponsY + 25,
             BORDER_ACCENT);

    const auto &weapons = player->GetWeapons();
    for (int i = 0; i < player->WEAPON_MAX; i++)
    {
        int weaponX =
            layout.weaponStartX + (i % layout.weaponsPerRow) * (layout.weaponSlotSize + layout.weaponSlotSpacing);
        int weaponY =
            layout.weaponStartY + (i / layout.weaponsPerRow) * (layout.weaponSlotSize + layout.weaponSlotSpacing);

        if (static_cast<size_t>(i) < weapons.size())
        {
            // Dibujar arma existente usando el struct unificado
            const auto &weapon = weapons[i];

            RenderableItem item = {
                WeaponTypeToItemType(weapon->GetWeaponType()), // itemType (convertir WEAPON_TYPE a ITEM_TYPE)
                weapon->GetName(),                             // name
                weapon->GetLevel(),                            // level
                true,                                          // isWeapon
                (i + Shop::MAX_ITEMS_SHOP == selectedItem),    // isSelected
                false,                                         // isBlocked
                0,                                             // price
                nullptr,                                       // stats
                "",                                            // description
                ItemRarity::Common                             // rarity
            };

            Rectangle slotRect = {(float)weaponX, (float)weaponY, (float)layout.weaponSlotSize,
                                  (float)layout.weaponSlotSize};
            RenderItemSlot(item, slotRect, false); // false = renderizado compacto
        }
        else
        {
            // Dibujar slot vacío
            DrawRectangle(weaponX - 2, weaponY - 2, layout.weaponSlotSize + 4, layout.weaponSlotSize + 4,
                          BORDER_SLOT_EMPTY);
            DrawRectangle(weaponX, weaponY, layout.weaponSlotSize, layout.weaponSlotSize, BG_SLOT_EMPTY);
            DrawText("EMPTY", weaponX + 10, weaponY + layout.weaponSlotSize / 2 - 7, 12, TEXT_DISABLED);
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
            int tooltipX = layout.statsX + 20;
            int tooltipY = layout.weaponStartY + layout.weaponSlotSize + layout.weaponSlotSpacing + 15;
            int tooltipWidth = layout.statsWidth - 40;
            int tooltipHeight = 120;

            // Fondo del tooltip
            DrawRectangle(tooltipX - 5, tooltipY - 5, tooltipWidth + 10, tooltipHeight + 10, BORDER_TOOLTIP);
            DrawRectangle(tooltipX, tooltipY, tooltipWidth, tooltipHeight, BG_TOOLTIP);

            // Título
            DrawText(weapon->GetName().c_str(), tooltipX + 10, tooltipY + 5, 16, TEXT_ACCENT);

            // Stats del arma
            int statsY = tooltipY + 30;
            const Stats &weaponStats = weapon->GetStats();

            auto drawWeaponStat = [&](const char *name, float value, int &currentY)
            {
                if (value != 0)
                {
                    Color color = value > 0 ? TEXT_POSITIVE : TEXT_NEGATIVE;
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
                    Color color = value > 0 ? TEXT_POSITIVE : TEXT_NEGATIVE;
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
                isMaxLevel ? BG_BUTTON_FUSE_DISABLED : BG_BUTTON_FUSE; // Gris si max level, morado si no
            Color fuseButtonBorder = isMaxLevel ? BORDER_BUTTON_FUSE_DISABLED
                                                : BORDER_BUTTON_FUSE; // Gris si max level, morado brillante si no

            // Borde negro si está en hover
            if (isHoveringFuse)
            {
                fuseButtonBorder = BORDER_HOVER;
            }

            DrawRectangle(buttonsX - 2, button1Y - 2, buttonWidth + 4, buttonHeight + 4, fuseButtonBorder);
            DrawRectangle(buttonsX, button1Y, buttonWidth, buttonHeight, fuseButtonColor);

            // Texto del botón fuse
            const char *fuseText = isMaxLevel ? "MAX" : "FUSE";
            int fuseTextWidth = MeasureText(fuseText, 14);
            Color fuseTextColor = isMaxLevel ? TEXT_DISABLED : WHITE;
            DrawText(fuseText, buttonsX + (buttonWidth - fuseTextWidth) / 2, button1Y + 8, 14, fuseTextColor);

            // Si no es el maximo nivel de arma
            if (!isMaxLevel)
            {

                // Leyenda de tecla dentro del botón
                const char *enterText = "[ENTER]";
                int enterTextWidth = MeasureText(enterText, 10);
                DrawText(enterText, buttonsX + (buttonWidth - enterTextWidth) / 2, button1Y + 26, 10, TEXT_HINT);
            }

            // Botón 2: SELL (abajo)
            int button2Y = buttonsStartY + buttonHeight + buttonSpacing;

            // Verificar hover en botón SELL
            Rectangle sellButtonRect = {(float)buttonsX, (float)button2Y, (float)buttonWidth, (float)buttonHeight};
            bool isHoveringSell = CheckCollisionPointRec(GetMousePosition(), sellButtonRect);

            // Fondo del botón de vender
            Color sellButtonColor = BG_BUTTON_SELL;      // Rojo oscuro
            Color sellButtonBorder = BORDER_BUTTON_SELL; // Rojo brillante

            // Borde negro si está en hover
            if (isHoveringSell)
            {
                sellButtonBorder = BORDER_HOVER;
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
            DrawText(spaceText, buttonsX + (buttonWidth - spaceTextWidth) / 2, button2Y + 26, 10, TEXT_CONTROLS);

            // Precio de venta
            int sellPrice = CalculateWeaponSellPrice(weaponIndex);
            DrawText("Sell Price:", tooltipX + 10, tooltipY + tooltipHeight - 25, 14, TEXT_SECONDARY);

            // Dibujar sprite de moneda
            int coinTextX = tooltipX + 10 + MeasureText("Sell Price: ", 14);
            Rectangle coinFrame = coinSheet.frames[0];

            // Centrar verticalmente la moneda con el texto (14 es el tamaño de la fuente)
            int textHeight = 14;
            int coinY = (tooltipY + tooltipHeight - 25) + (textHeight - (int)coinFrame.height) / 2;
            DrawTextureRec(coinSheet.texture, coinFrame, Vector2{(float)coinTextX, (float)coinY}, WHITE);

            // Dibujar cantidad con porcentaje (ajustar el espaciado según el ancho del sprite)
            int priceTextX = coinTextX + (int)coinFrame.width + 5;
            DrawText(TextFormat("%d (50%%)", sellPrice), priceTextX, tooltipY + tooltipHeight - 25, 14, TEXT_ACCENT);
        }
    }

    // Fondo del panel de items
    DrawRectangle(layout.itemsX - 5, layout.itemsY - 5, layout.itemsWidth + 10, layout.itemsHeight + 10, BORDER_ACCENT);
    DrawRectangle(layout.itemsX, layout.itemsY, layout.itemsWidth, layout.itemsHeight, BG_PANEL);

    // Título del panel
    DrawText("AVAILABLE ITEMS", layout.itemsX + 20, layout.itemsY + 15, 24, TEXT_ACCENT);
    DrawLine(layout.itemsX + 20, layout.itemsY + 45, layout.itemsX + layout.itemsWidth - 20, layout.itemsY + 45,
             BORDER_ACCENT);

    // Items
    for (int i = 0; i < Shop::MAX_ITEMS_SHOP; i++)
    {
        const TShopSlot &slot = shop.GetItemsShop()[i];

        if (slot.isBuyed)
            continue;

        int slotY = layout.itemStartY + i * (layout.itemSlotHeight + layout.itemSlotSpacing);

        // Determinar si es un arma
        ITEM_TYPE itemType = slot.item->GetType();
        bool isWeapon = IsWeaponType(itemType);
        // Calcular precio final (ajustado por nivel si es arma)
        int finalPrice = slot.item->GetPrice();
        if (isWeapon)
        {
            finalPrice *= slot.weaponLevel;
        }

        // Crear item renderizable usando el struct unificado
        const Stats &itemStats = slot.item->GetStats();

        RenderableItem item = {
            itemType,                        // itemType
            slot.item->GetName(),            // name
            isWeapon ? slot.weaponLevel : 0, // level
            isWeapon,                        // isWeapon
            (i == selectedItem),             // isSelected
            slot.isBlocked,                  // isBlocked
            finalPrice,                      // price
            &itemStats,                      // stats
            slot.item->GetDescription(),     // description
            slot.item->GetItemRarity()       // rarity
        };

        Rectangle slotRect = {(float)(layout.itemsX + 18), (float)slotY, (float)(layout.itemsWidth - 36),
                              (float)layout.itemSlotHeight};
        RenderItemSlot(item, slotRect, true); // true = renderizado completo con toda la info
    }

    // Botones de header
    int rerollButtonX = 50;
    int rerollButtonY = 20;
    Rectangle rerollButtonRect = {(float)rerollButtonX, (float)rerollButtonY, 150, 40};
    DrawButton(rerollButtonRect, "Reroll", player->GetPabloCoins() >= REROLL_COST, BG_BUTTON_ENABLED,
               BG_BUTTON_DISABLED, BORDER_BUTTON_ENABLED, BORDER_BUTTON_DISABLED, "[R]", REROLL_COST);

    int continueButtonX = rerollButtonX + 150 + 20;
    Rectangle continueButtonRect = {(float)continueButtonX, (float)rerollButtonY, 150, 40};
    DrawButton(continueButtonRect, "Continue", true, BG_BUTTON_CONTINUE, BG_BUTTON_CONTINUE, BORDER_BUTTON_CONTINUE,
               BORDER_BUTTON_CONTINUE, "[E]");

    // Controles en la parte inferior
    int controlsY = layout.screenHeight - 40;
    DrawRectangle(0, controlsY, layout.screenWidth, 40, BG_HEADER);
    DrawRectangle(0, controlsY, layout.screenWidth, 3, BORDER_ACCENT);
    const char *controlsText =
        "[W/S] Navigate  [ENTER/LEFT-CLICK] Buy  [SPACE/RIGHT-CLICK] Lock  [R] Reroll  [E] Continue";
    DrawText(controlsText, layout.screenWidth / 2 - MeasureText(controlsText, 16) / 2, controlsY + 12, 16,
             TEXT_CONTROLS);

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
