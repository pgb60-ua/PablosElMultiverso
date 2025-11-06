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
} OffensiveStatsComponent;

/// @brief Struct que representa las stats defensivas
typedef struct
{
    /// @brief Puntos de vida de la entidad
    float health;
    /// @brief Puntos de vida maximos de la entidad
    float healthMax;
    /// @brief Cantidad de puntos de vida que te curas cada segundo
    float healthRegeneration;
    /// @brief Probabilidad entre 0 - 30 de esquivar un golpe
    float agility;
    /// @brief Porcentaje entre 0 - 30 que reduce el daño recibido de tipo físico
    float armor;
    /// @brief Porcentaje entre 0 - 30 que reduce el daño recibido de tipo mágico
    float resistance;
} DefensiveStatsComponent;
