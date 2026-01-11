#include "AEntity.hpp"
#include <iostream>
#include <random>
#include <utility>

bool AEntity::Attack()
{
    return false;
}

AEntity::AEntity(Stats stats, const Shape &hitbox) : stats(std::move(stats)), hitbox(hitbox)
{
    // No se puede crear a una entidad con 0 de vida
    if (this->stats.GetHealth() <= 0)
        this->stats.SetHealth(MIN_HEALTH_BASE);
    // Ha de tener la misma vida maxima que la actual para que empiece full vida
    if (this->stats.GetMaxHealth() != this->stats.GetHealth())
        this->stats.SetMaxHealth(this->stats.GetHealth());
    // No se puede tener menos de 0 de attack speed
    if (this->stats.GetAttackSpeed() <= 0)
    {
        this->stats.SetAttackSpeed(0);
        this->attackCooldown = 0;
    }
    else
        this->attackCooldown = 1.0f / this->stats.GetAttackSpeed();
}

void AEntity::Render() {}

bool AEntity::IsAlive() { return stats.GetHealth() > 0; }

void AEntity::SetAttackSpeed(float newAttackSpeed)
{
    if (newAttackSpeed <= 0)
    {
        this->attackCooldown = 0;
        this->stats.SetAttackSpeed(0);
    }
    else
    {
        this->stats.SetAttackSpeed(newAttackSpeed);
        this->attackCooldown = 1.0f / newAttackSpeed;
    }
}

void AEntity::TakeDamage(const Stats &stats)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 99);

    // Aplicar evasión
    float evasion = this->stats.GetDefensiveStats().agility;
    if (dis(gen) < evasion)
    {
        animation.color = GRAY;
        return; // Se esquiva el ataque
    }

    // Reduce la salud basándose en las estadísticas de ataque recibidas
    float physicalDamage = stats.GetOffensiveStats().physicalDamage;
    float magicalDamage = stats.GetOffensiveStats().magicDamage;

    // Aplicar crítico
    float critChance = stats.GetOffensiveStats().criticalChance;
    float critMultiplier = 1.0f;

    if (dis(gen) < critChance)
    {
        critMultiplier = stats.GetOffensiveStats().criticalDamage;
        animation.color = ORANGE;
    }

    physicalDamage *= critMultiplier;
    magicalDamage *= critMultiplier;

    // Aplicar reducción de daño basada en la armadura
    float armor = this->stats.GetDefensiveStats().armor;
    float physicalDamageAfterArmor = physicalDamage * (100.0f / (100.0f + armor));

    // Aplicar reducción de daño basada en la resistencia mágica
    float magicResistance = this->stats.GetDefensiveStats().resistance;
    float magicalDamageAfterResistance = magicalDamage * (100.0f / (100.0f + magicResistance));

    float totalDamage = physicalDamageAfterArmor + magicalDamageAfterResistance;
    float newHealth = this->stats.GetHealth() - totalDamage;
    if (animation.color.r == WHITE.r && animation.color.g == WHITE.g && animation.color.b == WHITE.b &&
        animation.color.a == WHITE.a)
        animation.color = RED;
    this->stats.SetHealth(newHealth > 0 ? newHealth : 0);
}
