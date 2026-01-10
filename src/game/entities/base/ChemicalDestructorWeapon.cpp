#include "ChemicalDestructorWeapon.hpp"
#include "ChemicalDestructorProjectile.hpp"
#include "DataFileManager.hpp"
#include "Player.hpp"
#include "raylib.h"
#include "raymath.h"
#include <cmath>

ChemicalDestructorWeapon::ChemicalDestructorWeapon(const Vector2 &position, std::vector<AEnemy *> &enemies,
                                                   std::vector<Player *> &players)
    : ARangeWeapon(GetStringFromJSON("name", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, "Unknown Weapon"),
                   GetStringFromJSON("description", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, ""),
                   DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::CHEMICAL_DESTRUCTOR),
                   GetRarityFromJSON(WEAPON_TYPE::CHEMICAL_DESTRUCTOR),
                   GetIntFromJSON("level", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, 1),
                   GetIntFromJSON("pool_size", WEAPON_TYPE::CHEMICAL_DESTRUCTOR, 50), position, enemies, enemies),
      players(players)
{
    InitializeProjectilePool();
    SetPosition(position);
    SetWeaponType(WEAPON_TYPE::CHEMICAL_DESTRUCTOR);
}

ChemicalDestructorWeapon::~ChemicalDestructorWeapon() {}

Vector2 ChemicalDestructorWeapon::CalculateDirection()
{
    float minDistance = std::numeric_limits<float>::max();
    Vector2 closestPlayer = Vector2Zero();
    for (auto &player : players)
    {
        float distance = Vector2Distance(player->GetPosition(), position);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestPlayer = player->GetPosition();
        }
    }
    Vector2 directionToTarget = Vector2Subtract(closestPlayer, position);
    Vector2 directionToTargetNormalized = Vector2Normalize(directionToTarget);
    return directionToTargetNormalized;
}

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
    return std::make_unique<ChemicalDestructorProjectile>(allEnemies, players);
}

void ChemicalDestructorWeapon::Attack() { return; }
