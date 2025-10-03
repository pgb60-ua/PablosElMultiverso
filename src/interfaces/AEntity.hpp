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
  static constexpr float BASE_MULTIPLIER = 1.0f;

protected:
  /// @brief Vector de texturas de una la entidad
  std::vector<Texture2D *> textures;

  /// @brief Hitbox de la entidad (rectangulo - circulo - triangulo)
  Shape hitbox;

  /// @brief Puntos de vida de la entidad
  float health;
  /// @brief Modificador multiplicativo de vida
  float healthModifier = BASE_MULTIPLIER;

  /// @brief Cuantos pixeles se mueve por frame
  float movementSpeed;
  /// @brief Modificador multiplicativo de velocidad de movimiento
  float movementSpeedModifier = BASE_MULTIPLIER;

  /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
  float agility;
  /// @brief Modificador multiplicativo de agilidad
  float agilityModifier = BASE_MULTIPLIER;

  /// @brief Cuantos ataques por segundo se realizan
  float attackSpeed;
  /// @brief Modificador multiplicativo de velocidad de ataque
  float attackSpeedModifier = BASE_MULTIPLIER;

  /// @brief Propiedad autocalculada, tiempo que ha de pasar para volver atacar
  float attackCooldown;

  /// @brief Puntos de daño de tipo físico que se infligen al atacar
  float physicalDamage;
  /// @brief Modificador multiplicativo de daño físico
  float physicalDamageModifier = BASE_MULTIPLIER;

  /// @brief Puntos de daño de tipo mágico que se infligen al atacar
  float magicDamage;
  /// @brief Modificador multiplicativo de daño mágico
  float magicDamageModifier = BASE_MULTIPLIER;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// físico
  float armor;
  /// @brief Modificador multiplicativo de armadura
  float armorModifier = BASE_MULTIPLIER;

  /// @brief Probabilidad entre 0 - 30 que reduce el daño recibido de tipo
  /// mágico
  float resistance;
  /// @brief Modificador multiplicativo de resistencia mágica
  float resistanceModifier = BASE_MULTIPLIER;

  /// @brief Probabilidad entre 0 - 30 de aplicar más daño físico / mágico
  float criticalChance;
  /// @brief Modificador multiplicativo de probabilidad de crítico
  float criticalChanceModifier = BASE_MULTIPLIER;

  /// @brief Multiplicador entre 1 - 10 al daño infligido
  float criticalDamage;
  /// @brief Modificador multiplicativo de daño crítico
  float criticalDamageModifier = BASE_MULTIPLIER;

  /// @brief Porcentaje entre 0 - 50 de curación del daño al atacar
  float lifeSteal;
  /// @brief Modificador multiplicativo de robo de vida
  float lifeStealModifier = BASE_MULTIPLIER;

  /// @brief Cantidad de puntos de vida que te curas cada segundo
  float healthRegeneration;
  /// @brief Modificador multiplicativo de regeneración de vida
  float healthRegenerationModifier = BASE_MULTIPLIER;

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

  // Getters de modificadores
  /// @brief Obtiene el modificador de vida
  float GetHealthModifier() const { return healthModifier; }

  /// @brief Obtiene el modificador de velocidad de movimiento
  float GetMovementSpeedModifier() const { return movementSpeedModifier; }

  /// @brief Obtiene el modificador de agilidad
  float GetAgilityModifier() const { return agilityModifier; }

  /// @brief Obtiene el modificador de velocidad de ataque
  float GetAttackSpeedModifier() const { return attackSpeedModifier; }

  /// @brief Obtiene el modificador de daño físico
  float GetPhysicalDamageModifier() const { return physicalDamageModifier; }

  /// @brief Obtiene el modificador de daño mágico
  float GetMagicDamageModifier() const { return magicDamageModifier; }

  /// @brief Obtiene el modificador de armadura
  float GetArmorModifier() const { return armorModifier; }

  /// @brief Obtiene el modificador de resistencia mágica
  float GetResistanceModifier() const { return resistanceModifier; }

  /// @brief Obtiene el modificador de probabilidad de crítico
  float GetCriticalChanceModifier() const { return criticalChanceModifier; }

  /// @brief Obtiene el modificador de daño crítico
  float GetCriticalDamageModifier() const { return criticalDamageModifier; }

  /// @brief Obtiene el modificador de robo de vida
  float GetLifeStealModifier() const { return lifeStealModifier; }

  /// @brief Obtiene el modificador de regeneración de vida
  float GetHealthRegenerationModifier() const {
    return healthRegenerationModifier;
  }

  /**/
  // Setters de stats (aplican modificador automáticamente)

  /// @brief Establece los puntos de vida base y aplica modificador
  void SetHealth(float newHealth) { health = newHealth * healthModifier; }

  /// @brief Establece la velocidad de movimiento base y aplica modificador
  void SetMovementSpeed(float newSpeed) {
    movementSpeed = newSpeed * movementSpeedModifier;
  }

  /// @brief Establece la agilidad base y aplica modificador
  void SetAgility(float newAgility) { agility = newAgility * agilityModifier; }

  /// @brief Establece la velocidad de ataque base, aplica modificador y
  /// recalcula el tiempo de recarga
  void SetAttackSpeed(float newSpeed) {
    attackSpeed = newSpeed * attackSpeedModifier;
    attackCooldown = 1.0f / attackSpeed;
  }

  /// @brief Establece el daño físico base y aplica modificador
  void SetPhysicalDamage(float newDamage) {
    physicalDamage = newDamage * physicalDamageModifier;
  }

  /// @brief Establece el daño mágico base y aplica modificador
  void SetMagicDamage(float newDamage) {
    magicDamage = newDamage * magicDamageModifier;
  }

  /// @brief Establece la armadura base y aplica modificador
  void SetArmor(float newArmor) { armor = newArmor * armorModifier; }

  /// @brief Establece la resistencia mágica base y aplica modificador
  void SetResistance(float newResistance) {
    resistance = newResistance * resistanceModifier;
  }

  /// @brief Establece la probabilidad de crítico base y aplica modificador
  void SetCriticalChance(float newChance) {
    criticalChance = newChance * criticalChanceModifier;
  }

  /// @brief Establece el multiplicador de daño crítico base y aplica
  /// modificador
  void SetCriticalDamage(float newMultiplier) {
    criticalDamage = newMultiplier * criticalDamageModifier;
  }

  /// @brief Establece el porcentaje de robo de vida base y aplica modificador
  void SetLifeSteal(float newLifeSteal) {
    lifeSteal = newLifeSteal * lifeStealModifier;
  }

  /// @brief Establece la regeneración de vida base y aplica modificador
  void SetHealthRegeneration(float newRegeneration) {
    healthRegeneration = newRegeneration * healthRegenerationModifier;
  }

  /*--------------------------*/
  // Setters de modificadores
  /*--------------------------*/

  /// @brief Establece el modificador de vida
  void SetHealthModifier(float newModifier) { healthModifier = newModifier; }

  /// @brief Establece el modificador de velocidad de movimiento
  void SetMovementSpeedModifier(float newModifier) {
    movementSpeedModifier = newModifier;
  }

  /// @brief Establece el modificador de agilidad
  void SetAgilityModifier(float newModifier) { agilityModifier = newModifier; }

  /// @brief Establece el modificador de velocidad de ataque
  void SetAttackSpeedModifier(float newModifier) {
    attackSpeedModifier = newModifier;
  }

  /// @brief Establece el modificador de daño físico
  void SetPhysicalDamageModifier(float newModifier) {
    physicalDamageModifier = newModifier;
  }

  /// @brief Establece el modificador de daño mágico
  void SetMagicDamageModifier(float newModifier) {
    magicDamageModifier = newModifier;
  }

  /// @brief Establece el modificador de armadura
  void SetArmorModifier(float newModifier) { armorModifier = newModifier; }

  /// @brief Establece el modificador de resistencia mágica
  void SetResistanceModifier(float newModifier) {
    resistanceModifier = newModifier;
  }

  /// @brief Establece el modificador de probabilidad de crítico
  void SetCriticalChanceModifier(float newModifier) {
    criticalChanceModifier = newModifier;
  }

  /// @brief Establece el modificador de daño crítico
  void SetCriticalDamageModifier(float newModifier) {
    criticalDamageModifier = newModifier;
  }

  /// @brief Establece el modificador de robo de vida
  void SetLifeStealModifier(float newModifier) {
    lifeStealModifier = newModifier;
  }

  /// @brief Establece el modificador de regeneración de vida
  void SetHealthRegenerationModifier(float newModifier) {
    healthRegenerationModifier = newModifier;
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
  void SetTriangleHitbox(float v1, float v2, float v3) {
    hitbox.type = SHAPE_RECTANGLE;
    hitbox.data.triangle = {v1, v2, v3};
  };

  /// @brief Establece la hitbox de la entidad a un circulo
  void SetCircleHitbox(float x, float y, float radius) {
    hitbox.type = SHAPE_CIRCLE;
    hitbox.data = {x, y, radius};
  };

  void Render();
  virtual void TakeDamage(float amount) = 0;
  virtual void Update() = 0;
  virtual bool IsAlive();
  virtual ~AEntity() { textures.clear(); };
};
