#include "Zombie.hpp"
#include <cmath>
#include <limits>

Zombie::Zombie(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath)
    : AEnemy(stats, hitbox, objectives, pabloCoinsAtDeath)
{
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
}

void Zombie::Move(float deltaTime)
{
    Player *closestPlayer = GetClosestPlayer();

    // Si no hay jugadores, no se mueve
    if (closestPlayer == nullptr)
        return;
    // TODO: Obtener posición del jugador más cercano
    // TODO: Implementar cuando se tenga acceso a la posición del jugador
}

int Zombie::DropLoot() const { return pabloCoinsAtDeath; }

Player *Zombie::GetClosestPlayer()
{
    if (objectives.empty())
        return nullptr;

    // TODO: Implementar cuando Player tenga método GetPosition()
    // Por ahora retorna el primer jugador
    return objectives[0];
}
