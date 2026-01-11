#include "AEnemy.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <raymath.h>

AEnemy::AEnemy(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath)
    : AEntity(stats, hitbox), objectives(objectives), pabloCoinsAtDeath(pabloCoinsAtDeath)
{
    s_allEnemies.push_back(this);

    const float baseSpeed = stats.GetMovementSpeed();
    Vector2 randomDir = {((float)std::rand() / (float)RAND_MAX) * 2.0f - 1.0f,
                         ((float)std::rand() / (float)RAND_MAX) * 2.0f - 1.0f};
    if (Vector2Length(randomDir) <= 0.001f)
    {
        randomDir = Vector2{1.0f, 0.0f};
    }

    velocity = Vector2Scale(Vector2Normalize(randomDir), baseSpeed * 0.5f);
}

AEnemy::~AEnemy()
{
    auto it = std::find(s_allEnemies.begin(), s_allEnemies.end(), this);
    if (it != s_allEnemies.end())
    {
        s_allEnemies.erase(it);
    }
}

void AEnemy::SetObjective(const std::vector<Player *> newObjectives) { objectives = newObjectives; }

void AEnemy::SetPabloCoinsAtDeath(int cantidad) { pabloCoinsAtDeath = cantidad; }

int AEnemy::GetPabloCoinsAtDeath() const { return pabloCoinsAtDeath; }

void AEnemy::DropLoot()
{
    for (Player *player : objectives)
    {
        if (player->IsAlive())
        {
            player->ModifyPabloCoins(pabloCoinsAtDeath);
        }
    }
}

Player *AEnemy::GetClosestPlayer()
{
    if (objectives.empty())
        return nullptr;

    // Búsqueda del jugador más cercano
    float minDistance = std::numeric_limits<float>::max();
    int closestIndex = -1;

    for (size_t i = 0; i < objectives.size(); ++i)
    {
        Vector2 playerPos = objectives[i]->GetPosition();
        float distance = Vector2Distance(GetPosition(), playerPos);
        if (distance < minDistance)
        {
            minDistance = distance;
            closestIndex = static_cast<int>(i);
        }
    }

    return objectives[closestIndex];
}

void AEnemy::UpdateEnemyAnimation(float deltaTime, ENEMY_TYPE enemyType)
{

    animation.timeAccumulator += deltaTime;

    if (animation.timeAccumulator >= animation.FRAME_DURATION)
    {
        animation.timeAccumulator -= animation.FRAME_DURATION;
        animation.frameIndex++;
        animation.frameIndex %= SpriteLoaderManager::GetInstance().GetSpriteSheet(enemyType).spriteFrameCount;
    }
}

void AEnemy::CheckCollisions(float deltaTime) {}

Vector2 AEnemy::CalculateTargetForce(const Vector2 &enemyPos, const Vector2 &playerPos, float baseSpeed)
{
    Vector2 toPlayer = Vector2Subtract(playerPos, enemyPos);
    if (Vector2Length(toPlayer) > 0.0f)
    {
        toPlayer = Vector2Scale(Vector2Normalize(toPlayer), baseSpeed * targetWeight);
        return toPlayer;
    }
    return Vector2Zero();
}

void AEnemy::Move(float deltaTime)
{
    if (!IsAlive())
        return;
    Player *closestPlayer = GetClosestPlayer();

    // Si no hay jugadores, no se mueve
    if (closestPlayer == nullptr)
        return;

    Vector2 enemyPos = GetPosition();

    Vector2 separationForce = Vector2Zero();
    Vector2 alignmentForce = Vector2Zero();
    Vector2 cohesionForce = Vector2Zero();

    int neighborCount = 0;

    for (AEnemy *other : s_allEnemies)
    {
        if (other == this)
            continue;

        Vector2 offset = Vector2Subtract(other->GetPosition(), enemyPos);
        float distance = Vector2Length(offset);

        if (distance <= 0.0f || distance > perceptionRadius)
            continue;

        neighborCount++;
        cohesionForce = Vector2Add(cohesionForce, other->GetPosition());
        alignmentForce = Vector2Add(alignmentForce, other->velocity);

        if (distance < separationRadius)
        {
            Vector2 away = Vector2Scale(Vector2Normalize(offset), -1.0f);
            float falloff = (separationRadius - distance) / separationRadius;
            separationForce = Vector2Add(separationForce, Vector2Scale(away, falloff));
        }
    }

    Vector2 acceleration = Vector2Zero();
    const float baseSpeed = stats.GetMovementSpeed();

    if (neighborCount > 0)
    {
        cohesionForce = Vector2Scale(cohesionForce, 1.0f / neighborCount);
        cohesionForce = Vector2Subtract(cohesionForce, enemyPos);
        if (Vector2Length(cohesionForce) > 0.0f)
        {
            cohesionForce = Vector2Scale(Vector2Normalize(cohesionForce), baseSpeed * cohesionWeight);
            acceleration = Vector2Add(acceleration, cohesionForce);
        }

        alignmentForce = Vector2Scale(alignmentForce, 1.0f / neighborCount);
        if (Vector2Length(alignmentForce) > 0.0f)
        {
            alignmentForce = Vector2Scale(Vector2Normalize(alignmentForce), baseSpeed * alignmentWeight);
            acceleration = Vector2Add(acceleration, alignmentForce);
        }
    }

    if (Vector2Length(separationForce) > 0.0f)
    {
        separationForce = Vector2Scale(Vector2Normalize(separationForce), baseSpeed * separationWeight);
        acceleration = Vector2Add(acceleration, separationForce);
    }

    // Calcular fuerza hacia el objetivo usando método virtual (permite personalización)
    Vector2 targetForce = CalculateTargetForce(enemyPos, closestPlayer->GetPosition(), baseSpeed);
    acceleration = Vector2Add(acceleration, targetForce);

    const float maxForce = baseSpeed * maxForceMultiplier;
    if (Vector2Length(acceleration) > maxForce)
    {
        acceleration = Vector2Scale(Vector2Normalize(acceleration), maxForce);
    }

    velocity = Vector2Add(velocity, Vector2Scale(acceleration, deltaTime));

    const float maxSpeed = baseSpeed;
    float currentSpeed = Vector2Length(velocity);
    if (currentSpeed > maxSpeed)
    {
        velocity = Vector2Scale(Vector2Normalize(velocity), maxSpeed);
    }

    Vector2 newPos = Vector2Add(enemyPos, Vector2Scale(velocity, deltaTime));
    SetPosition(newPos);
}
