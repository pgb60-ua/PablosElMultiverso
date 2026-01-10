#pragma once
enum class PLAYER_TYPE
{
    WARRIOR,
    MAGE,
    RANGE,
    HEALER
};

enum class ENEMY_TYPE
{
    ZOMBIE,
    DARKIN,
    CHEMICAL_DESTRUCTOR
};

enum class ITEM_TYPE
{
    FIREBALL,
    SPINED_BREASTPLATE,
    PEARL_EGG,
    RAVENS_FEATHER,
    VOID_RING,
    SATCHEL,
    SPIRIT_MASK,
    CELESTIAL_SPARK,
    EXPLOSIVE_ARSENAL,
    MAGICAL_MIRROR,
    ETHEREAL_DAGGER,
    SPOTLIGHT,
    CRIMSON_VIAL,
    RUBY_HEART,
    RAW_MEAT,
    VENOMOUS_PLANT,
    COIN,
    WEAPON_AXE,
    WEAPON_SWORD,
    WEAPON_SCYTHE,
    WEAPON_WAND,
    WEAPON_EGGPLOSIVE,
    WEAPON_LASER_RAY,
    WEAPON_SNIPER,
    WEAPON_WING
};

enum class WEAPON_TYPE
{
    AXE,
    SWORD,
    SCYTHE,
    WAND,
    EGGPLOSIVE,
    LASER_RAY,
    SNIPER,
    WING
};

enum class PROJECTILE_TYPE
{
    WAND,
    EGGPLOSIVE_CIRCLE,
    EGGPLOSIVE_BULLET,
    LASER_RAY,
    SNIPER,
    WING,
    CHEMICAL_BULLET
};

enum class MAP_TYPE
{
    DEFAULT,
    DEFAULT_UPPER
};

enum class ROUND_TYPE
{
    EASY,
    MEDIUM,
    HARD
};

// Función helper para verificar si un ITEM_TYPE es un arma
inline bool IsWeaponType(ITEM_TYPE type) { return (type >= ITEM_TYPE::WEAPON_AXE && type <= ITEM_TYPE::WEAPON_WING); }

// Función helper para convertir ITEM_TYPE a WEAPON_TYPE
inline WEAPON_TYPE ItemTypeToWeaponType(ITEM_TYPE itemType)
{
    switch (itemType)
    {
    case ITEM_TYPE::WEAPON_AXE:
        return WEAPON_TYPE::AXE;
    case ITEM_TYPE::WEAPON_SWORD:
        return WEAPON_TYPE::SWORD;
    case ITEM_TYPE::WEAPON_SCYTHE:
        return WEAPON_TYPE::SCYTHE;
    case ITEM_TYPE::WEAPON_WAND:
        return WEAPON_TYPE::WAND;
    case ITEM_TYPE::WEAPON_EGGPLOSIVE:
        return WEAPON_TYPE::EGGPLOSIVE;
    case ITEM_TYPE::WEAPON_LASER_RAY:
        return WEAPON_TYPE::LASER_RAY;
    case ITEM_TYPE::WEAPON_SNIPER:
        return WEAPON_TYPE::SNIPER;
    case ITEM_TYPE::WEAPON_WING:
        return WEAPON_TYPE::WING;
    default:
        return WEAPON_TYPE::AXE; // Fallback
    }
}
