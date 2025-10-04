#include "AEntity.hpp"

AEntity::AEntity(Stats stats, bool canShoot, const Shape &hitbox,
                 std::vector<Texture2D *> textures)
    : stats(std::move(stats)), canShoot(canShoot), hitbox(hitbox),
      textures(textures) {
  // No se puede crear a una entidad con 0 de vida
  if (this->stats.GetHealth() <= 0)
    this->stats.SetHealth(MIN_HEALTH);
  // No se puede crear a una entidad con regeneracion negativa
  if (this->stats.GetHealthRegeneration() < 0)
    this->stats.SetHealthRegeneration(MIN_REGENERATION);
  // Si la entidad puede disparar calculamos su cooldown
  if (canShoot)
    attackCooldown = 1.0f / stats.GetAttackSpeed();
  else
    attackCooldown = 0;
}

void AEntity::Render() {}

bool AEntity::IsAlive() { return stats.GetHealth() > 0; }

bool AEntity::CanAttack() { return stats.GetAttackSpeed() > 0; }
