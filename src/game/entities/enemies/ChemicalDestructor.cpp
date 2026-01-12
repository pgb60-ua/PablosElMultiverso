#include "ChemicalDestructor.hpp"
#include "AEnemy.hpp"
#include "ChemicalDestructorWeapon.hpp"
#include "ScreenConstants.hpp"
#include "raymath.h"
#include <vector>

ChemicalDestructor::ChemicalDestructor(std::vector<Player *> players)
    : AEnemy(DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::CHEMICAL_DESTRUCTOR),
             SpriteLoaderManager::GetInstance().GetSpriteHitbox(
                 ENEMY_TYPE::CHEMICAL_DESTRUCTOR,
                 Vector2{(float)(std::rand() % ENEMY_SCREEN_WIDTH), (float)(std::rand() % ENEMY_SCREEN_HEIGHT)}),
             ENEMY_TYPE::CHEMICAL_DESTRUCTOR, players, 50)
{
    targetWeight = 3.5f;
    static std::vector<AEnemy *> emptyVector;
    Vector2 enemyCenter = GetCenterPosition();
    this->weapon = std::make_unique<ChemicalDestructorWeapon>(enemyCenter, emptyVector, this->objectives);
}

bool ChemicalDestructor::Attack()
{
    // Implementar el ataque
    return true;
}

void ChemicalDestructor::Update(float deltaTime)
{
    AEnemy::Update(deltaTime);
    if (weapon)
    {
        Vector2 enemyCenter = GetCenterPosition();
        weapon->update(deltaTime, enemyCenter);
    }
}

Vector2 ChemicalDestructor::GetCenterPosition() const
{
    return Vector2{hitbox.data.rectangle.x + hitbox.data.rectangle.width * 0.5f,
                   hitbox.data.rectangle.y + hitbox.data.rectangle.height * 0.5f};
}

Vector2 ChemicalDestructor::CalculateTargetForce(const Vector2 &enemyPos, const Vector2 &playerPos, float baseSpeed)
{
    // Mantener distancia del jugador en lugar de acercarse infinitamente
    Vector2 toPlayer = Vector2Subtract(playerPos, enemyPos);
    float distance = Vector2Length(toPlayer);

    if (distance <= 0.0f)
        return Vector2Zero();

    Vector2 directionToPlayer = Vector2Normalize(toPlayer);
    float distanceDifference = distance - TARGET_DISTANCE;

    // Si está muy cerca, alejarse; si está muy lejos, acercarse
    // Usar un factor de suavizado para evitar movimientos bruscos
    float forceMultiplier = (distanceDifference / TARGET_DISTANCE) * targetWeight;

    return Vector2Scale(directionToPlayer, baseSpeed * forceMultiplier);
}

void ChemicalDestructor::Render()
{
    AEnemy::Render();

    if (weapon)
    {
        weapon->render();
    }
}
