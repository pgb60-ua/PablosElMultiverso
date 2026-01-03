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
    ENEMY2,
    ENEMY3,
    ENEMY4
};

enum class ITEM_TYPE
{
    ITEM1,
    ITEM2,
    ITEM3,
    WEAPON
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
    WING
};

enum class MAP_TYPE
{
    DEFAULT,
    DEFAULT_UPPER
};
