#pragma once

#include "Geometry.hpp"
#include <vector>
extern "C" {
#include "raylib.h"
}

// Clase que representa lo mas abstracto de un personaje y enemigo que tienen
// común
class AEntity {
private:
  static constexpr float MIN_HEALTH = 1.0f;
  static constexpr float MIN_REGENERATION = 0.0f;

protected:
  /// @brief Vector de texturas de una la entidad
  std::vector<Texture2D *> textures;

  /// @brief Hitbox de la entidad (rectangulo - circulo - triangulo)
  Shape hitbox;

  /// @brief Puntos de vida de la entidad
  float health;

  /// @brief Cuantos pixeles se mueve por frame
  float movementSpeed;

  /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
  float agility;

  /// @brief Cuantos ataques por segundo se realizan
  float attackSpeed;

  /// @brief Propiedad autocalculada, tiempo que ha de pasar para volver atacar
  float attackCooldown;

  /// @brief Puntos de daño de tipo físico que se infligen al atacar
  float physicalDamage;

  /// @brief Puntos de daño de tipo mágico que se infligen al atacar
  float magicDamage;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// físico
  float armor;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// mágico
  float resistance;

  /// @brief Probabilidad entre 0 - 30 de aplicar más daño físico / mágico
  float criticalChance;

  /// @brief Multiplicador entre 1 - 10 al daño infligido
  float criticalDamage;

  /// @brief Porcentaje entre 0 - 50 de curación del daño al atacar
  float lifeSteal;

  /// @brief Cantidad de puntos de vida que te curas cada segundo
  float healthRegeneration;

  AEntity(float health, float movementSpeed, float agility, float attackSpeed,
          float physicalDamage, float magicDamage, float armor,
          float resistance, float criticalChance, float criticalDamage,
          float lifeSteal, float healthRegeneration,
          std::vector<Texture2D *> textures);

public:
  // Getters de stats
  /// @brief Obtiene los puntos de vida actuales
  float GetHealth() const { return health; }

  /// @brief Obtiene la velocidad de movimiento actual
  float GetMovementSpeed() const { return movementSpeed; }

  /// @brief Obtiene la agilidad actual
  float GetAgility() const { return agility; }

  /// @brief Obtiene la velocidad de ataque actual
  float GetAttackSpeed() const { return attackSpeed; }

  /// @brief Obtiene el tiempo de recarga del ataque actual
  float GetAttackCooldown() const { return attackCooldown; }

  /// @brief Obtiene el daño físico actual
  float GetPhysicalDamage() const { return physicalDamage; }

  /// @brief Obtiene el daño mágico actual
  float GetMagicDamage() const { return magicDamage; }

  /// @brief Obtiene la armadura actual
  float GetArmor() const { return armor; }

  /// @brief Obtiene la resistencia mágica actual
  float GetResistance() const { return resistance; }

  /// @brief Obtiene la probabilidad de crítico actual
  float GetCriticalChance() const { return criticalChance; }

  /// @brief Obtiene el multiplicador de daño crítico actual
  float GetCriticalDamage() const { return criticalDamage; }

  /// @brief Obtiene el porcentaje de robo de vida actual
  float GetLifeSteal() const { return lifeSteal; }

  /// @brief Obtiene la regeneración de vida por segundo actual
  float GetHealthRegeneration() const { return healthRegeneration; }

  // Setters de stats

  /// @brief Establece los puntos de vida
  void SetHealth(float newHealth) { health = newHealth; }

  /// @brief Establece la velocidad de movimiento
  void SetMovementSpeed(float newSpeed) { movementSpeed = newSpeed; }

  /// @brief Establece la agilidad
  void SetAgility(float newAgility) { agility = newAgility; }

  /// @brief Establece la velocidad de ataque y recalcula el tiempo de recarga
  void SetAttackSpeed(float newSpeed) {
    attackSpeed = newSpeed;
    attackCooldown = 1.0f / attackSpeed;
  }

  /// @brief Establece el daño físico
  void SetPhysicalDamage(float newDamage) { physicalDamage = newDamage; }

  /// @brief Establece el daño mágico
  void SetMagicDamage(float newDamage) { magicDamage = newDamage; }

  /// @brief Establece la armadura
  void SetArmor(float newArmor) { armor = newArmor; }

  /// @brief Establece la resistencia mágica
  void SetResistance(float newResistance) { resistance = newResistance; }

  /// @brief Establece la probabilidad de crítico
  void SetCriticalChance(float newChance) { criticalChance = newChance; }

  /// @brief Establece el multiplicador de daño crítico
  void SetCriticalDamage(float newMultiplier) {
    criticalDamage = newMultiplier;
  }

  /// @brief Establece el porcentaje de robo de vida
  void SetLifeSteal(float newLifeSteal) { lifeSteal = newLifeSteal; }

  /// @brief Establece la regeneración de vida
  void SetHealthRegeneration(float newRegeneration) {
    healthRegeneration = newRegeneration;
  }

  /*--------------------------*/
  // Hitbox
  /*--------------------------*/

  /// @brief Devuelve la hitbox de la entidad
  Shape GetHitbox() { return hitbox; };

  /// @brief Establece la hitbox de la entidad a un rectangulo
  void SetRectangleHitbox(Rectangle rectangle) {
    hitbox.type = SHAPE_RECTANGLE;
    hitbox.data.rectangle = rectangle;
  };

  /// @brief Establece la hitbox de la entidad a un triangulo
  void SetTriangleHitbox(Vector2 v1, Vector2 v2, Vector2 v3) {
    hitbox.type = SHAPE_TRIANGLE;
    hitbox.data.triangle = {v1, v2, v3};
  };

  /// @brief Establece la hitbox de la entidad a un circulo
  void SetCircleHitbox(float x, float y, float radius) {
    hitbox.type = SHAPE_CIRCLE;
    hitbox.data.circle = {x, y, radius};
  };

  void Render();
  virtual void TakeDamage(float amount) = 0;
  virtual void Update() = 0;
  virtual bool IsAlive();
  virtual ~AEntity() { textures.clear(); };
};
