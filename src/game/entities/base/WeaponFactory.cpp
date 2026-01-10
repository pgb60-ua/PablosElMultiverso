#include "WeaponFactory.hpp"
#include "EggplosiveWeapon.hpp"
#include "LaserRayWeapon.hpp"
#include "SniperWeapon.hpp"
#include "Types.hpp"
#include "WingWeapon.hpp"

std::unique_ptr<AWeapon> WeaponFactory::CreateWeapon(ITEM_TYPE itemType, const Vector2 &position,
                                                     std::vector<AEnemy *> &enemiesInRange,
                                                     std::vector<AEnemy *> &allEnemies)
{
    switch (itemType)
    {
    case ITEM_TYPE::WEAPON_EGGPLOSIVE:
        return std::make_unique<EggplosiveWeapon>(position, enemiesInRange, allEnemies);
    case ITEM_TYPE::WEAPON_LASER_RAY:
        return std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies);
    case ITEM_TYPE::WEAPON_SNIPER:
        return std::make_unique<SniperWeapon>(position, enemiesInRange, allEnemies);
    case ITEM_TYPE::WEAPON_WING:
        return std::make_unique<WingWeapon>(position, enemiesInRange, allEnemies);
    default:
        return nullptr;
    }
}

std::unique_ptr<AWeapon> WeaponFactory::CreateStartingWeapon(PLAYER_TYPE playerType, const Vector2 &position,
                                                             std::vector<AEnemy *> &enemiesInRange,
                                                             std::vector<AEnemy *> &allEnemies)
{
    switch (playerType)
    {
    case PLAYER_TYPE::WARRIOR:
        return std::make_unique<WingWeapon>(position, enemiesInRange, allEnemies);
    case PLAYER_TYPE::MAGE:
        return std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies);
    case PLAYER_TYPE::RANGE:
        return std::make_unique<SniperWeapon>(position, enemiesInRange, allEnemies);
    case PLAYER_TYPE::HEALER:
        return std::make_unique<EggplosiveWeapon>(position, enemiesInRange, allEnemies);
    default:
        return std::make_unique<LaserRayWeapon>(position, enemiesInRange, allEnemies); // Default weapon
    }
}
