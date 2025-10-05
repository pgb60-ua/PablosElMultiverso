#include "Stats.hpp"

Stats::Stats(float health, float healthMax, float movementSpeed, float agility,
             float attackSpeed, float physicalDamage, float magicDamage,
             float armor, float resistance, float criticalChance,
             float criticalDamage, float lifeSteal, float healthRegeneration)
    : health(health), healthMax(healthMax), movementSpeed(movementSpeed),
      agility(agility), attackSpeed(attackSpeed),
      physicalDamage(physicalDamage), magicDamage(magicDamage), armor(armor),
      resistance(resistance), criticalChance(criticalChance),
      criticalDamage(criticalDamage), lifeSteal(lifeSteal),
      healthRegeneration(healthRegeneration) {};
