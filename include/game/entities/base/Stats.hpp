#pragma once

/// @brief Struct que representa las stats ofensivas
typedef struct
{
    /// @brief Puntos de daño de tipo físico que se infligen al atacar
    float physicalDamage;
    /// @brief Puntos de daño de tipo mágico que se infligen al atacar
    float magicDamage;
    /// @brief Cuantos ataques por segundo se realizan
    float attackSpeed;
    /// @brief Probabilidad entre 0 - 30 de aplicar más daño físico / mágico
    float criticalChance;
    /// @brief Multiplicador entre 1 - 10 al daño critico infligido
    float criticalDamage;
    /// @brief Porcentaje entre 0 - 50 de curación del daño al atacar
    float lifeSteal;
} OffensiveStats;

/// @brief Struct que representa las stats defensivas
typedef struct
{
    /// @brief Puntos de vida de la entidad
    float health;
    /// @brief Puntos de vida maximos de la entidad
    float healthMax;
    /// @brief Cuantos pixeles se mueve por frame
    float movementSpeed;
    /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
    float agility;
    /// @brief Porcentaje entre 0 - 30 que reduce el daño recibido de tipo físico
    float armor;
    /// @brief Porcentaje entre 0 - 30 que reduce el daño recibido de tipo mágico
    float resistance;
    /// @brief Cantidad de puntos de vida que te curas cada segundo
    float healthRegeneration;
} DefensiveStats;

// Clase que almacena estatisticas
class Stats
{
protected:
    OffensiveStats offensiveStats;

    DefensiveStats defensiveStats;

public:
    Stats(const OffensiveStats &offensiveStats, const DefensiveStats &defensiveStats);

    /// @brief Constructor por defecto que inicializa todas las stats a 0 (usado para proyectiles)
    Stats();

    // Getters de stats
    /// @brief Obtiene los puntos de vida actuales
    float GetHealth() const { return defensiveStats.health; }

    /// @brief Obtiene los puntos de vida maxima
    float GetMaxHealth() const { return defensiveStats.healthMax; };

    /// @brief Obtiene la velocidad de movimiento actual
    float GetMovementSpeed() const { return defensiveStats.movementSpeed; }

    /// @brief Obtiene la agilidad actual
    float GetAgility() const { return defensiveStats.agility; }

    /// @brief Obtiene la velocidad de ataque actual
    float GetAttackSpeed() const { return offensiveStats.attackSpeed; }

    /// @brief Obtiene el daño físico actual
    float GetPhysicalDamage() const { return offensiveStats.physicalDamage; }

    /// @brief Obtiene el daño mágico actual
    float GetMagicDamage() const { return offensiveStats.magicDamage; }

    /// @brief Obtiene la armadura actual
    float GetArmor() const { return defensiveStats.armor; }

    /// @brief Obtiene la resistencia mágica actual
    float GetResistance() const { return defensiveStats.resistance; }

    /// @brief Obtiene la probabilidad de crítico actual
    float GetCriticalChance() const { return offensiveStats.criticalChance; }

    /// @brief Obtiene el multiplicador de daño crítico actual
    float GetCriticalDamage() const { return offensiveStats.criticalDamage; }

    /// @brief Obtiene el porcentaje de robo de vida actual
    float GetLifeSteal() const { return offensiveStats.lifeSteal; }

    /// @brief Obtiene la regeneración de vida por segundo actual
    float GetHealthRegeneration() const { return defensiveStats.healthRegeneration; }

    // Setters de stats

    /// @brief Establece los puntos de vida base
    void SetHealth(float newHealth) { defensiveStats.health = newHealth; }

    /// @brief Establece los puntos de vida maxima
    void SetMaxHealth(float newHealthMax) { defensiveStats.healthMax = newHealthMax; };

    /// @brief Establece la velocidad de movimiento base
    void SetMovementSpeed(float newSpeed) { defensiveStats.movementSpeed = newSpeed; }

    /// @brief Establece la agilidad base
    void SetAgility(float newAgility) { defensiveStats.agility = newAgility; }

    /// @brief Establece la velocidad de ataque base
    void SetAttackSpeed(float newSpeed) { offensiveStats.attackSpeed = newSpeed; }

    /// @brief Establece el daño físico base
    void SetPhysicalDamage(float newDamage) { offensiveStats.physicalDamage = newDamage; }

    /// @brief Establece el daño mágico base
    void SetMagicDamage(float newDamage) { offensiveStats.magicDamage = newDamage; }

    /// @brief Establece la armadura base
    void SetArmor(float newArmor) { defensiveStats.armor = newArmor; }

    /// @brief Establece la resistencia mágica base
    void SetResistance(float newResistance) { defensiveStats.resistance = newResistance; }

    /// @brief Establece la probabilidad de crítico base
    void SetCriticalChance(float newChance) { offensiveStats.criticalChance = newChance; }

    /// @brief Establece el multiplicador de daño crítico base
    void SetCriticalDamage(float newMultiplier) { offensiveStats.criticalDamage = newMultiplier; }

    /// @brief Establece el porcentaje de robo de vida base
    void SetLifeSteal(float newLifeSteal) { offensiveStats.lifeSteal = newLifeSteal; }

    /// @brief Establece la regeneración de vida base
    void SetHealthRegeneration(float newRegeneration) { defensiveStats.healthRegeneration = newRegeneration; }

    /// @brief Devuelve las estadísticas ofensivas agrupadas
    OffensiveStats GetOffensiveStats() const { return offensiveStats; }
    /// @brief Devuelve las estadísticas defensivas agrupadas
    DefensiveStats GetDefensiveStats() const { return defensiveStats; }

    void SetOffensiveStats(const OffensiveStats &offensiveStats);
    void SetDefensiveStats(const DefensiveStats &defensiveStats);

    ~Stats() = default;
};
