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
  if (health < MIN_HEALTH)
    health = MIN_HEALTH;
  // La regeneracion de vida como minimo es 0
  if (healthRegeneration < MIN_REGENERATION)
    healthRegeneration = MIN_REGENERATION;
};

void AEntity::Render() {}

void AEntity::SetTriangleHitbox(float v1, float v2, float v3) {
  hitbox.type = SHAPE_RECTANGLE;
  hitbox.data.triangle = {v1, v2, v3};
}

void AEntity::SetRectangleHitbox(Rectangle rectangle) {
  hitbox.type = SHAPE_RECTANGLE;
  hitbox.data.rectangle = rectangle;
}

void AEntity::SetCircleHitbox(float x, float y, float radius) {
  hitbox.type = SHAPE_CIRCLE;
  hitbox.data = {x, y, radius};
}

Shape AEntity::GetHitbox() { return hitbox; }

AEntity::~AEntity() {
  // Las texturas son compartidas y gestionadas externamente,
  // solo limpiamos el vector de punteros
  textures.clear();
}
