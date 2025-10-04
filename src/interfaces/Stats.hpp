#pragma once

// @brief Struct que representa las stats ofensivas
typedef struct {
  float physicalDamage;
  float magicDamage;
  float attackSpeed;
  float criticalChance;
  float criticalDamage;
  float lifeSteal;
} OffensiveStats;

/// @brief Struct que representa las stats defensivas
typedef struct {
  float health;
  float movementSpeed;
  float agility;
  float armor;
  float resistance;
  float healthRegeneration;
} DefensiveStats;

// Clase que almacena estatisticas
class Stats {
private:
protected:
  /// @brief Puntos de vida de la entidad
  float health;

  /// @brief Cuantos pixeles se mueve por frame
  float movementSpeed;

  /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
  float agility;

  /// @brief Cuantos ataques por segundo se realizan
  float attackSpeed;

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

  Stats(float health, float movementSpeed, float agility, float attackSpeed,
        float physicalDamage, float magicDamage, float armor, float resistance,
        float criticalChance, float criticalDamage, float lifeSteal,
        float healthRegeneration);

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

  /**/
  // Setters de stats (aplican modificador automáticamente)

  /// @brief Establece los puntos de vida base y aplica modificador
  void SetHealth(float newHealth) { health = newHealth; }

  /// @brief Establece la velocidad de movimiento base y aplica modificador
  void SetMovementSpeed(float newSpeed) { movementSpeed = newSpeed; }

  /// @brief Establece la agilidad base y aplica modificador
  void SetAgility(float newAgility) { agility = newAgility; }

  /// @brief Establece la velocidad de ataque base, aplica modificador y
  /// recalcula el tiempo de recarga
  void SetAttackSpeed(float newSpeed) { attackSpeed = newSpeed; }

  /// @brief Establece el daño físico base y aplica modificador
  void SetPhysicalDamage(float newDamage) { physicalDamage = newDamage; }

  /// @brief Establece el daño mágico base y aplica modificador
  void SetMagicDamage(float newDamage) { magicDamage = newDamage; }

  /// @brief Establece la armadura base y aplica modificador
  void SetArmor(float newArmor) { armor = newArmor; }

  /// @brief Establece la resistencia mágica base y aplica modificador
  void SetResistance(float newResistance) { resistance = newResistance; }

  /// @brief Establece la probabilidad de crítico base y aplica modificador
  void SetCriticalChance(float newChance) { criticalChance = newChance; }

  /// @brief Establece el multiplicador de daño crítico base y aplica
  /// modificador
  void SetCriticalDamage(float newMultiplier) {
    criticalDamage = newMultiplier;
  }

  /// @brief Establece el porcentaje de robo de vida base y aplica modificador
  void SetLifeSteal(float newLifeSteal) { lifeSteal = newLifeSteal; }

  /// @brief Establece la regeneración de vida base y aplica modificador
  void SetHealthRegeneration(float newRegeneration) {
    healthRegeneration = newRegeneration;
  }

  /// @brief Devuelve las estadísticas ofensivas agrupadas
  OffensiveStats GetOffensiveStats() const {
    return {physicalDamage, magicDamage,    attackSpeed,
            criticalChance, criticalDamage, lifeSteal};
  }
  /// @brief Devuelve las estadísticas defensivas agrupadas
  DefensiveStats GetDefensiveStats() const {
    return {health, movementSpeed, agility,
            armor,  resistance,    healthRegeneration};
  }

  virtual ~Stats() {

  };
};
