#include "AEntity.hpp"

AEntity::AEntity(float health, float movementSpeed, float agility,
                 float attackSpeed, float physicalDamage, float magicDamage,
                 float armor, float resistance, float criticalChance,
                 float criticalDamage, float lifeSteal,
                 float healthRegeneration, std::vector<Texture2D *> textures)
    : health(health), movementSpeed(movementSpeed), agility(agility),
      attackSpeed(attackSpeed), physicalDamage(physicalDamage),
      magicDamage(magicDamage), armor(armor), resistance(resistance),
      criticalChance(criticalChance), criticalDamage(criticalDamage),
      lifeSteal(lifeSteal), healthRegeneration(healthRegeneration),
      attackCooldown(1.0f / attackSpeed), textures(textures) {
  // La vida como minimo es 1
  if (this->health < MIN_HEALTH)
    this->health = MIN_HEALTH;
  // La regeneracion de vida como minimo es 0
  if (this->healthRegeneration < MIN_REGENERATION)
    this->healthRegeneration = MIN_REGENERATION;
};

void AEntity::Render() {}

bool AEntity::IsAlive() {}
