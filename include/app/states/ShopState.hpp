#pragma once

#include "Player.hpp"
#include "Shop.hpp"
#include <GameState.hpp>

extern "C"
{
#include "raylib.h"
}

typedef struct
{
    Color borderColor;
    Color tintColor;
} TWeaponColor;

// Struct unificado para renderizar cualquier tipo de item (tienda o inventario)
struct RenderableItem
{
    ITEM_TYPE itemType;
    std::string name;
    Rectangle spriteFrame;
    Texture2D texture;
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
    void NextSelectedItem(int direction);
    /// @brief devuelve a que arma salto del inventario
    void NextWeaponSelected(int direction);
    int CalculateWeaponSellPrice(int weaponIndex);

    // Funciones helper para renderizado unificado
    TWeaponColor GetColorBasedOnWeaponLevel(int level);
    void RenderItemSlot(const RenderableItem &item, Rectangle slotRect, bool showFullInfo);
    void DrawItemSprite(const RenderableItem &item, int x, int y, int maxSize);
    void DrawButton(Rectangle rect, const char *text, bool isEnabled, Color enabledBg, Color disabledBg,
                    Color enabledBorder, Color disabledBorder, const char *keyHint = nullptr, int price = -1);
    void DrawStatWithMultiplier(const char *name, float value, float multiplier, int &y, int x, int spacing);

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
