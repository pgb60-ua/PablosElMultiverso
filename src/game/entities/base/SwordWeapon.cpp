#include "SwordWeapon.hpp"
#include <cmath>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"
#include "SpriteLoaderManager.hpp"
#include "AudioManager.hpp"

SwordWeapon::SwordWeapon(const Vector2 &position, std::vector<AEnemy *> &enemiesInRange, std::vector<AEnemy *> &allEnemies)
    : AMeleeWeapon(
          GetStringFromJSON("name", WEAPON_TYPE::SWORD, ""),
          GetStringFromJSON("description", WEAPON_TYPE::SWORD, ""),
          DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::SWORD),
          GetRarityFromJSON(WEAPON_TYPE::SWORD),
          GetIntFromJSON("level", WEAPON_TYPE::SWORD, 1),
          position,
          enemiesInRange,
          allEnemies)
{
    SetWeaponType(WEAPON_TYPE::SWORD);
}

SwordWeapon::~SwordWeapon()
{
}

void SwordWeapon::Attack()
{
    // Implementación requerida por pura
}

void SwordWeapon::Attack(float deltaTime)
{
    timeSinceLastAttack += deltaTime;

    if (isThrusting)
    {
        thrustTimer += deltaTime;
        
        float progress = thrustTimer / attackInterval;
        
        if (progress >= 1.0f)
        {
            isThrusting = false;
            currentThrustAmount = 0.0f;
            hitEnemies.clear();
        }
        else
        {
            currentThrustAmount = sinf(progress * PI) * THRUST_DISTANCE;
            
            Vector2 thrustDir = direction;
            Vector2 thrustVec = Vector2Scale(thrustDir, currentThrustAmount);
            Vector2 attackPos = Vector2Add(position, thrustVec);
            
            float spriteSize = DEFAULT_ATTACK_SIZE; 
            const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::SWORD);
            if (!sheet.frames.empty()) {
                spriteSize = std::max(sheet.frames[0].width, sheet.frames[0].height);
            }
            
            Rectangle attackRect = {
                attackPos.x - spriteSize * 0.5f,
                attackPos.y - spriteSize * 0.5f,
                spriteSize,
                spriteSize
            };

            if (!enemiesInRange.empty())
            {
                enemiesInRange.erase(
                    std::remove(enemiesInRange.begin(), enemiesInRange.end(), nullptr),
                    enemiesInRange.end());

                for (AEnemy *enemy : enemiesInRange)
                {
                    if (!enemy || !enemy->IsAlive()) continue;

                    if (std::find(hitEnemies.begin(), hitEnemies.end(), enemy) != hitEnemies.end())
                    {
                        continue;
                    }

                    if (CheckCollisionRecs(attackRect, enemy->GetHitbox().data.rectangle))
                    {
                        enemy->TakeDamage(stats);
                        hitEnemies.push_back(enemy);
                    }
                }
                SetPosition(Vector2Add(GetPosition(), thrustVec));
            }
        }
    }
    else if (timeSinceLastAttack >= attackInterval)
    {
        isThrusting = true;
        thrustTimer = 0.0f; 
        currentThrustAmount = 0.0f;
        timeSinceLastAttack -= attackInterval;
        hitEnemies.clear();
        AudioManager::GetInstance().PlaySound(WEAPON_TYPE::SWORD);
    }
}

void SwordWeapon::update(float deltaTime, const Vector2 &playerPos)
{
    AMeleeWeapon::update(deltaTime, playerPos);
}

/*
void SwordWeapon::render()
{
    AWeapon::render();

    if (isThrusting)
    {
        float size = DEFAULT_ATTACK_SIZE;
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::SWORD);
        if (!sheet.frames.empty()) {
            size = std::max(sheet.frames[0].width, sheet.frames[0].height);
        }
        
        Rectangle r = {
            position.x - size * 0.5f,
            position.y - size * 0.5f,
            size,
            size
        };
        DrawRectangleLinesEx(r, 2.0f, RED);
    }
}
*/
