#include "ChemicalDestructorWeapon.hpp"
#include "ChemicalDestructorProjectile.hpp"
#include "DataFileManager.hpp"
#include <cmath>

ChemicalDestructorWeapon::ChemicalDestructorWeapon(const Vector2 &position, std::vector<AEnemy *> &enemiesInRange,
                                                   std::vector<AEnemy *> &allEnemies)
    : ARangeWeapon(GetStringFromJSON("name", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, "Unknown Weapon"),
                   GetStringFromJSON("description", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, ""),
                   DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::CHEMICAL_DESTRUCTOR),
                   GetRarityFromJSON(WEAPON_TYPE::CHEMICAL_DESTRUCTOR),
                   GetIntFromJSON("level", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, 1),
                   GetIntFromJSON("pool_size", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, 50), position, enemiesInRange,
                   allEnemies)
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::CHEMICAL_DESTRUCTOR);
}

ChemicalDestructorWeapon::~ChemicalDestructorWeapon() {}

void ChemicalDestructorWeapon::Attack(const Vector2 &position, float deltaTime)
{
    timeSinceLastAttack += deltaTime;
    if (timeSinceLastAttack >= attackInterval)
    {
        ShootProjectile(position, direction, allEnemies);
        timeSinceLastAttack -= attackInterval;
    }
}

std::unique_ptr<AProjectile> ChemicalDestructorWeapon::CreateProjectile()
{
    return std::make_unique<ChemicalDestructorProjectile>(allEnemies);
}
