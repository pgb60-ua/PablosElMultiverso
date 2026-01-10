#include "AxeWeapon.hpp"
#include <cmath>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"
#include "SpriteLoaderManager.hpp"

AxeWeapon::AxeWeapon(const Vector2 &position, std::vector<AEnemy *> &enemiesInRange, std::vector<AEnemy *> &allEnemies)
    : AMeleeWeapon(
          GetStringFromJSON("name", WEAPON_TYPE::AXE, ""),
          GetStringFromJSON("description", WEAPON_TYPE::AXE, ""),
          DataFileManager::GetInstance().GetWeaponStats(WEAPON_TYPE::AXE),
          GetRarityFromJSON(WEAPON_TYPE::AXE),
          GetIntFromJSON("level", WEAPON_TYPE::AXE, 1),
          position,
          enemiesInRange,
          allEnemies)
{
    SetWeaponType(WEAPON_TYPE::AXE);
}

AxeWeapon::~AxeWeapon()
{
}

void AxeWeapon::Attack()
{
    // Implementación requerida por la interfaz pura
}

void AxeWeapon::Attack(float deltaTime)
{
    timeSinceLastAttack += deltaTime;

    if (isSwinging)
    {
        float oneWayDuration = swingRange / swingSpeed;
        float totalDuration = oneWayDuration * 2.0f;

        if (timeSinceLastAttack <= oneWayDuration)
        {
            float progress = timeSinceLastAttack / oneWayDuration;
            swingAngle = progress * swingRange;
            
            const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::AXE);
            float spriteSize = DEFAULT_ATTACK_RANGE;
            if (!sheet.frames.empty()) {
                spriteSize = std::max(sheet.frames[0].width, sheet.frames[0].height);
            }
            float attackRange = spriteSize; 
            float baseAngleRad = atan2(direction.y, direction.x);
            float baseAngleDeg = baseAngleRad * RAD2DEG;

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

                    Vector2 enemyPos = enemy->GetPosition();
                    enemyPos.x += enemy->GetHitbox().data.rectangle.width / 2.0f;
                    enemyPos.y += enemy->GetHitbox().data.rectangle.height / 2.0f;

                    Vector2 toEnemy = Vector2Subtract(enemyPos, position); 
                    float dist = Vector2Length(toEnemy);

                    if (dist <= attackRange)
                    {
                        float enemyAngle = atan2(toEnemy.y, toEnemy.x) * RAD2DEG;
                        float angleDiff = enemyAngle - baseAngleDeg;
                        
                        while (angleDiff <= -180.0f) angleDiff += 360.0f;
                        while (angleDiff > 180.0f) angleDiff -= 360.0f;

                        float startSwing = -swingRange / 2.0f;
                        float currentEndSwing = startSwing + swingAngle;

                        if (angleDiff >= startSwing && angleDiff <= currentEndSwing)
                        {
                            enemy->TakeDamage(stats);
                            hitEnemies.push_back(enemy);
                        }
                    }
                }
            }
        }
        else
        {
            float returnTime = timeSinceLastAttack - oneWayDuration;
            float progress = returnTime / oneWayDuration;
            swingAngle = swingRange - (progress * swingRange);
        }

        // Finalizar ataque
        if (timeSinceLastAttack >= totalDuration)
        {
            isSwinging = false;
            swingAngle = 0.0f;
            hitEnemies.clear();
        }
    }
    else if (timeSinceLastAttack >= attackInterval)
    {
        isSwinging = true;
        swingAngle = 0.0f; 
        timeSinceLastAttack = 0.0f;
        hitEnemies.clear();
    }

    float currentDirAngle = atan2(direction.y, direction.x);
    float startOffset = -swingRange / 2.0f;
    float finalVisualAngle = currentDirAngle + ((startOffset + swingAngle) * DEG2RAD);
    
    SetDirection(Vector2{cos(finalVisualAngle), sin(finalVisualAngle)});
}

void AxeWeapon::update(float deltaTime, const Vector2 &position)
{
    AMeleeWeapon::update(deltaTime, position);
}

/*
void AxeWeapon::render()
{
    AWeapon::render();

    if (isSwinging)
    {
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(WEAPON_TYPE::AXE);
        float spriteSize = 50.0f;
        if (!sheet.frames.empty()) {
            spriteSize = std::max(sheet.frames[0].width, sheet.frames[0].height);
        }
        float attackRange = spriteSize;
        
        Vector2 endPos = Vector2Add(position, Vector2Scale(direction, attackRange));
        DrawLineEx(position, endPos, 3.0f, RED);
    }
}
Debug para ver el rango del arma*/
