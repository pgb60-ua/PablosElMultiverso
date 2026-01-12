#include "WeaponFactory.hpp"
#include "AxeWeapon.hpp"
#include "EggplosiveWeapon.hpp"
#include "LaserRayWeapon.hpp"
#include "SniperWeapon.hpp"
#include "SwordWeapon.hpp"
#include "Types.hpp"
#include "WingWeapon.hpp"

std::unique_ptr<AWeapon> WeaponFactory::CreateWeapon(ITEM_TYPE itemType, const Vector2 &position,
                                                     std::vector<AEnemy *> &enemiesInRange,
                                                     std::vector<AEnemy *> &allEnemies, int price, int level)
{
    std::unique_ptr<AWeapon> weapon;

    switch (itemType)
    {
    case ITEM_TYPE::WEAPON_EGGPLOSIVE:
        weapon = std::make_unique<EggplosiveWeapon>(position, enemiesInRange, allEnemies);
        break;
    case ITEM_TYPE::WEAPON_LASER_RAY:
        weapon = std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies);
        break;
    case ITEM_TYPE::WEAPON_SNIPER:
        weapon = std::make_unique<SniperWeapon>(position, enemiesInRange, allEnemies);
        break;
    case ITEM_TYPE::WEAPON_WING:
        weapon = std::make_unique<WingWeapon>(position, enemiesInRange, allEnemies);
        break;
    case ITEM_TYPE::WEAPON_AXE:
        weapon = std::make_unique<AxeWeapon>(position, enemiesInRange, allEnemies);
        break;
    case ITEM_TYPE::WEAPON_SWORD:
        weapon = std::make_unique<SwordWeapon>(position, enemiesInRange, allEnemies);
        break;
    default:
        return nullptr;
    }

    if (weapon)
    {
        weapon->SetPrice(price);
        weapon->SetLevel(level);
    }

    return weapon;
}

std::unique_ptr<AWeapon> WeaponFactory::CreateStartingWeapon(PLAYER_TYPE playerType, const Vector2 &position,
                                                             std::vector<AEnemy *> &enemiesInRange,
                                                             std::vector<AEnemy *> &allEnemies, int price)
{
    std::unique_ptr<AWeapon> weapon;

    switch (playerType)
    {
    case PLAYER_TYPE::WARRIOR:
        weapon = std::make_unique<SwordWeapon>(position, enemiesInRange, allEnemies);
        break;
    case PLAYER_TYPE::MAGE:
        weapon = std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies);
        break;
    case PLAYER_TYPE::RANGE:
        weapon = std::make_unique<WingWeapon>(position, enemiesInRange, allEnemies);
        break;
    case PLAYER_TYPE::HEALER:
        weapon = std::make_unique<EggplosiveWeapon>(position, enemiesInRange, allEnemies);
        break;
    default:
        weapon = std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies); // Default weapon
        break;
    }

    if (weapon)
    {
        weapon->SetPrice(price);
    }

    return weapon;
}
