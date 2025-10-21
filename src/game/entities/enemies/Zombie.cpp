#include "Zombie.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <raymath.h>

Zombie::Zombie(std::vector<Player *> objectives)
    : AEnemy(
            DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::ZOMBIE),
            SpriteLoaderManager::GetInstance().GetSpriteHitbox(ENEMY_TYPE::ZOMBIE,
            Vector2{(float)(std::rand() % 2000), (float)(std::rand() % 2000)}),
            objectives,
            50)
{
    // Las stats se cargan automáticamente desde zombie.json en la lista de inicialización
    s_allZombies.push_back(this);

    const float baseSpeed = stats.GetMovementSpeed();
    Vector2 randomDir = {((float)std::rand() / (float)RAND_MAX) * 2.0f - 1.0f,
                         ((float)std::rand() / (float)RAND_MAX) * 2.0f - 1.0f};
    if (Vector2Length(randomDir) <= 0.001f)
    {
        randomDir = Vector2{1.0f, 0.0f};
    }

    velocity = Vector2Scale(Vector2Normalize(randomDir), baseSpeed * 0.5f);
}

void Zombie::TakeDamage(float amount)
{
    // Reduce la salud
    // TODO:
    float newHealth = stats.GetHealth() - amount;
    stats.SetHealth(newHealth > 0 ? newHealth : 0);
}

bool Zombie::Attack()
{
    if (currentAttackCooldownTime >= attackCooldown)
    {
        currentAttackCooldownTime -= attackCooldown;
        return true;
    }

    return false; // Los zombies solo hacen daño al colisionar
}

void Zombie::Move(float deltaTime)
{
    Player *closestPlayer = GetClosestPlayer();

    // Si no hay jugadores, no se mueve
    if (closestPlayer == nullptr)
        return;

    Vector2 zombiePos = GetPosition();

    Vector2 separationForce = Vector2Zero();
    Vector2 alignmentForce = Vector2Zero();
    Vector2 cohesionForce = Vector2Zero();

    int neighborCount = 0;

    for (Zombie *other : s_allZombies)
    {
        if (other == this)
            continue;

        Vector2 offset = Vector2Subtract(other->GetPosition(), zombiePos);
        float distance = Vector2Length(offset);

        if (distance <= 0.0f || distance > PERCEPTION_RADIUS)
            continue;

        neighborCount++;
        cohesionForce = Vector2Add(cohesionForce, other->GetPosition());
        alignmentForce = Vector2Add(alignmentForce, other->velocity);

        if (distance < SEPARATION_RADIUS)
        {
            Vector2 away = Vector2Scale(Vector2Normalize(offset), -1.0f);
            float falloff = (SEPARATION_RADIUS - distance) / SEPARATION_RADIUS;
            separationForce = Vector2Add(separationForce, Vector2Scale(away, falloff));
        }
    }

    Vector2 acceleration = Vector2Zero();
    const float baseSpeed = stats.GetMovementSpeed();

    if (neighborCount > 0)
    {
        cohesionForce = Vector2Scale(cohesionForce, 1.0f / neighborCount);
        cohesionForce = Vector2Subtract(cohesionForce, zombiePos);
        if (Vector2Length(cohesionForce) > 0.0f)
        {
            cohesionForce = Vector2Scale(Vector2Normalize(cohesionForce), baseSpeed * COHESION_WEIGHT);
            acceleration = Vector2Add(acceleration, cohesionForce);
        }

        alignmentForce = Vector2Scale(alignmentForce, 1.0f / neighborCount);
        if (Vector2Length(alignmentForce) > 0.0f)
        {
            alignmentForce = Vector2Scale(Vector2Normalize(alignmentForce), baseSpeed * ALIGNMENT_WEIGHT);
            acceleration = Vector2Add(acceleration, alignmentForce);
        }
    }

    if (Vector2Length(separationForce) > 0.0f)
    {
        separationForce = Vector2Scale(Vector2Normalize(separationForce), baseSpeed * SEPARATION_WEIGHT);
        acceleration = Vector2Add(acceleration, separationForce);
    }

    Vector2 toPlayer = Vector2Subtract(closestPlayer->GetPosition(), zombiePos);
    if (Vector2Length(toPlayer) > 0.0f)
    {
        toPlayer = Vector2Scale(Vector2Normalize(toPlayer), baseSpeed * TARGET_WEIGHT);
        acceleration = Vector2Add(acceleration, toPlayer);
    }

    const float maxForce = baseSpeed * MAX_FORCE_MULTIPLIER;
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

    Vector2 newPos = Vector2Add(zombiePos, Vector2Scale(velocity, deltaTime));
    SetPosition(newPos);
}

void Zombie::Render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(ENEMY_TYPE::ZOMBIE);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

    Rectangle dest = {hitbox.data.rectangle.x, hitbox.data.rectangle.y,
                      src.width, src.height};

    DrawTexturePro(sheet.texture, src, dest, origin, 0, WHITE);
}

std::vector<Zombie *> Zombie::s_allZombies;

Zombie::~Zombie()
{
    auto it = std::find(s_allZombies.begin(), s_allZombies.end(), this);
    if (it != s_allZombies.end())
    {
        s_allZombies.erase(it);
    }
}

void Zombie::Update(float deltaTime)
{
    // Actualiza el cooldown de ataque
    currentAttackCooldownTime += deltaTime;

    Move(deltaTime);

    // Regeneración de vida
    if (stats.GetHealthRegeneration() > 0 && IsAlive())
    {
        float newHealth = stats.GetHealth() + (stats.GetHealthRegeneration() * deltaTime);
        if (newHealth > stats.GetMaxHealth())
            newHealth = stats.GetMaxHealth();
        stats.SetHealth(newHealth);
    }
    UpdateEnemyAnimation(deltaTime, ENEMY_TYPE::ZOMBIE);
}
