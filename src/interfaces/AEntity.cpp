#include "AEntity.hpp"

AEntity::AEntity(Stats stats, const Shape &hitbox,
                 std::vector<Texture2D *> textures)
    : stats(std::move(stats)), hitbox(hitbox), textures(textures) {
  // No se puede crear a una entidad con 0 de vida
  if (this->stats.GetHealth() <= 0)
    this->stats.SetHealth(MIN_HEALTH_BASE);
  // Ha de tener la misma vida maxima que la actual para que empiece full vida
  if (this->stats.GetMaxHealth() != this->stats.GetHealth())
    this->stats.SetMaxHealth(this->stats.GetHealth());
  // No se puede crear a una entidad con regeneracion negativa
  if (this->stats.GetHealthRegeneration() < 0)
    this->stats.SetHealthRegeneration(MIN_REGENERATION_BASE);
  // No se puede tener menos de 0 de attack speed
  if (this->stats.GetAttackSpeed() <= 0) {
    this->stats.SetAttackSpeed(0);
    this->attackCooldown = 0;
  } else
    this->attackCooldown = 1.0f / this->stats.GetAttackSpeed();
}

void AEntity::Render() {}

bool AEntity::IsAlive() { return stats.GetHealth() > 0; }

void AEntity::SetHealthMax(float newHealthMax) {
  if (newHealthMax <= 0)
    newHealthMax = 1;
  this->stats->SetMaxHealth(newHealthMax);
}

void AEntity::SetAttackSpeed(float newAttackSpeed) {
  if (newAttackSpeed <= 0) {
    this->attackCooldown = 0;
    this->stats.SetAttackSpeed(0);
  }
}
