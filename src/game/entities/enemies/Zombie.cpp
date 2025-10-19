#include "Zombie.hpp"
#include <cmath>
#include <limits>
#include <raymath.h>

Zombie::Zombie(std::vector<Player *> objectives)
    : AEnemy(DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::ZOMBIE), SpriteLoaderManager::GetInstance().GetSpriteHitbox(ENEMY_TYPE::ZOMBIE, Vector2{(float)(std::rand() % 2000), (float)(std::rand() % 2000)}), objectives, 50)
{
    // Las stats se cargan automáticamente desde zombie.json en la lista de inicialización
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

void Zombie::Move(float deltaTime)
{
    Player *closestPlayer = GetClosestPlayer();

    // Si no hay jugadores, no se mueve
    if (closestPlayer == nullptr)
        return;

    Vector2 playerPos = closestPlayer->GetPosition();
    Vector2 zombiePos = GetPosition();

    // Moverse hacia el jugador más cercano
    Vector2 direction = Vector2Normalize(Vector2Subtract(playerPos, zombiePos));
    SetPosition(Vector2Add(zombiePos, Vector2Scale(direction, stats.GetMovementSpeed() * deltaTime)));
}

int Zombie::DropLoot() const { return pabloCoinsAtDeath; }

Player *Zombie::GetClosestPlayer()
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