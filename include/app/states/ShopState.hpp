#pragma once

#include "Player.hpp"
#include "Shop.hpp"
#include <GameState.hpp>

extern "C"
{
#include "raylib.h"
}

struct TWeaponColor
{
    Color borderColor;
    Color tintColor;
};

// Struct para almacenar las constantes de layout de la tienda
struct ShopLayout
{
    int screenWidth;
    int screenHeight;
    int headerHeight;
    int statsX;
    int statsY;
    int statsWidth;
    int statsHeight;
    int itemsX;
    int itemsY;
    int itemsWidth;
    int itemsHeight;
    int itemSlotHeight;
    int itemSlotSpacing;
    int itemStartY;
    int weaponSlotSize;
    int weaponSlotSpacing;
    int weaponsPerRow;
    int statSpacing;
    int weaponPanelY;
    int weaponsY;
    int weaponStartX;
    int weaponStartY;
};

// Struct unificado para renderizar cualquier tipo de item (tienda o inventario)
struct RenderableItem
{
    ITEM_TYPE itemType;
    std::string name;
    int level; // Para armas (0 si no es arma)
    bool isWeapon;
    bool isSelected;
    bool isBlocked;          // Para items de tienda
    int price;               // Para items de tienda (0 si es inventario)
    const Stats *stats;      // Puntero a las stats del item
    std::string description; // Para items de tienda (vacío si es inventario)
    ItemRarity rarity;       // Para items de tienda
};

class ShopState : public GameState
{
private:
    bool passRound = false;
    Player *player;
    Shop shop;
    // 0, 1, 2, 3, 4 - Items de compra
    // 5, 6, 7, 8 - Inventario de armas
    int selectedItem = 0;
    bool willBuy = false;
    bool willAlternateBlock = false;
    bool willReroll = false;
    bool willFuse = false;
    bool willSell = false;
    constexpr static int PADDING = 100;
    constexpr static int REROLL_COST = 5;

    // Color constants - Weapon levels
    const Color WEAPON_LEVEL1_BORDER = {150, 150, 150, 255};
    const Color WEAPON_LEVEL1_TINT = {150, 150, 150, 80};
    const Color WEAPON_LEVEL2_BORDER = {100, 255, 100, 255};
    const Color WEAPON_LEVEL2_TINT = {100, 255, 100, 80};
    const Color WEAPON_LEVEL3_BORDER = {200, 100, 255, 255};
    const Color WEAPON_LEVEL3_TINT = {200, 100, 255, 80};
    const Color WEAPON_LEVEL4_BORDER = {255, 200, 50, 255};
    const Color WEAPON_LEVEL4_TINT = {255, 200, 50, 80};

    // Color constants - UI backgrounds
    const Color BG_MAIN = {20, 20, 30, 255};
    const Color BG_HEADER = {40, 40, 60, 255};
    const Color BG_PANEL = {30, 30, 45, 255};
    const Color BG_SLOT = {45, 45, 65, 255};
    const Color BG_SLOT_SELECTED = {60, 80, 120, 255};
    const Color BG_SLOT_BLOCKED = {80, 40, 40, 255};
    const Color BG_SLOT_EMPTY = {60, 60, 70, 255};
    const Color BG_TOOLTIP = {40, 40, 60, 255};
    const Color BG_BUTTON_ENABLED = {60, 100, 60, 255};
    const Color BG_BUTTON_DISABLED = {80, 40, 40, 255};
    const Color BG_BUTTON_REROLL_DISABLED = {60, 60, 70, 255};
    const Color BG_BUTTON_FUSE = {80, 60, 120, 255};
    const Color BG_BUTTON_FUSE_DISABLED = {60, 60, 70, 255};
    const Color BG_BUTTON_SELL = {180, 40, 40, 255};
    const Color BG_BUTTON_CONTINUE = {60, 80, 120, 255};

    // Color constants - Borders
    const Color BORDER_ACCENT = {255, 200, 0, 255};
    const Color BORDER_DEFAULT = {80, 80, 100, 255};
    const Color BORDER_SELECTED = {100, 150, 255, 255};
    const Color BORDER_BLOCKED = {180, 60, 60, 255};
    const Color BORDER_SLOT_EMPTY = {80, 80, 100, 255};
    const Color BORDER_HOVER = {0, 0, 0, 255};
    const Color BORDER_BUTTON_ENABLED = {100, 200, 100, 255};
    const Color BORDER_BUTTON_DISABLED = {180, 60, 60, 255};
    const Color BORDER_BUTTON_FUSE = {150, 100, 255, 255};
    const Color BORDER_BUTTON_FUSE_DISABLED = {80, 80, 90, 255};
    const Color BORDER_BUTTON_SELL = {255, 80, 80, 255};
    const Color BORDER_BUTTON_CONTINUE = {100, 150, 255, 255};
    const Color BORDER_TOOLTIP = {100, 150, 255, 255};

    // Color constants - Text
    const Color TEXT_PRIMARY = {255, 255, 255, 255};
    const Color TEXT_ACCENT = {255, 200, 0, 255};
    const Color TEXT_SECONDARY = {200, 200, 220, 255};
    const Color TEXT_TERTIARY = {180, 180, 200, 255};
    const Color TEXT_HINT = {120, 120, 120, 255};
    const Color TEXT_DISABLED = {120, 120, 130, 255};
    const Color TEXT_POSITIVE = {100, 255, 100, 255};
    const Color TEXT_NEGATIVE = {255, 100, 100, 255};
    const Color TEXT_NEUTRAL = {150, 150, 150, 255};
    const Color TEXT_CONTROLS = {200, 200, 200, 255};

    // Color constants - Rarity
    const Color RARITY_COMMON = {150, 150, 150, 255};
    const Color RARITY_UNCOMMON = {100, 255, 100, 255};
    const Color RARITY_RARE = {100, 150, 255, 255};
    const Color RARITY_EPIC = {200, 100, 255, 255};
    const Color RARITY_LEGENDARY = {255, 180, 50, 255};

    // Color constants - Special
    const Color MULTIPLIER_POSITIVE = {100, 255, 100, 255};
    const Color MULTIPLIER_NEGATIVE = {255, 100, 100, 255};
    const Color MULTIPLIER_NEUTRAL = {150, 150, 150, 255};
    const Color WEAPON_TAG_COLOR = {255, 200, 100, 255};
    void NextSelectedItem(int direction);
    /// @brief devuelve a que arma salto del inventario
    void NextWeaponSelected(int direction);
    int CalculateWeaponSellPrice(int weaponIndex);

    // Funciones helper para renderizado unificado
    ShopLayout CalculateLayout();
    TWeaponColor GetColorBasedOnWeaponLevel(int level);
    void RenderItemSlot(const RenderableItem &item, Rectangle slotRect, bool showFullInfo);
    void DrawItemSprite(const RenderableItem &item, int x, int y, int maxSize);
    void DrawButton(Rectangle rect, const char *text, bool isEnabled, Color enabledBg, Color disabledBg,
                    Color enabledBorder, Color disabledBorder, const char *keyHint = nullptr, int price = -1);
    void DrawStatWithMultiplier(const char *name, float value, float multiplier, int &y, int x, int spacing,
                                int statsWidth);

public:
    ShopState(Player *player);
    ~ShopState();

    void init() override {};
    void handleInput() override;
    void update(float deltaTime) override;
    void render() override;

    void pause() override {};
    void resume() override {};
};
