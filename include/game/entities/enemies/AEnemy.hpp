#pragma once

#include "AEntity.hpp"
#include "Player.hpp"
#include "Types.hpp"
#include <vector>
extern "C"
{
#include <raylib.h>
}

class Player;
/// @brief Interfaz que representa el comportamiento de un enemigo en el juego.
/// Los enemigos son entidades que atacan al jugador, se mueven con IA y
/// sueltan botín al morir
class AEnemy : public AEntity
{
protected:
    /// @brief Objetivo actual del enemigo (posición del jugador)
    std::vector<Player *> objectives;

    /// @brief Cantidad de Pablo Coins que suelta al morir
    int pabloCoinsAtDeath;

    /// @brief Registra todas las instancias activas para aplicar las reglas de Boids
    inline static std::vector<AEnemy *> s_allEnemies;

    /// @brief Velocidad actual acumulada del enemigo para suavizar el movimiento
    Vector2 velocity;

    ENEMY_TYPE type;

    /*--------------------------*/
    // Parámetros de Boids (personalizables por cada enemigo)
    /*--------------------------*/

    /// @brief Radio de percepción para detectar otros enemigos cercanos
    float perceptionRadius = 250.0f;
    /// @brief Radio de separación para evitar aglomeración con otros enemigos
    float separationRadius = 80.0f;
    /// @brief Peso de la regla de alineación (seguir dirección del grupo)
    float alignmentWeight = 0.5f;
    /// @brief Peso de la regla de cohesión (moverse hacia el centro del grupo)
    float cohesionWeight = 0.45f;
    /// @brief Peso de la regla de separación (evitar colisiones con otros enemigos)
    float separationWeight = 0.75f;
    /// @brief Peso de atracción hacia el objetivo (jugador)
    float targetWeight = 1.2f;
    /// @brief Multiplicador máximo de fuerza para limitar aceleraciones abruptas
    float maxForceMultiplier = 1.6f;

    /// @brief Constructor protegido para clases derivadas
    AEnemy(Stats stats, const Shape &hitbox, ENEMY_TYPE type, std::vector<Player *> objectives, int pabloCoinsAtDeath);
    /// @brief Actualiza la animación del enemigo
    void UpdateEnemyAnimation(float deltaTime, ENEMY_TYPE enemyType);

    /// @brief Calcula la fuerza hacia el objetivo (jugador)
    virtual Vector2 CalculateTargetForce(const Vector2 &enemyPos, const Vector2 &playerPos, float baseSpeed);

public:
    /*--------------------------*/
    // Movimiento e IA
    /*--------------------------*/

    /// @brief Actualiza el estado del enemigo (animación, cooldowns, etc.)
    virtual void Update(float deltaTime) override;

    /// @brief Mueve el enemigo hacia su objetivo usando algoritmo de Boids
    /// @param deltaTime Tiempo transcurrido desde el último frame
    virtual void Move(float deltaTime);

    /// @brief Establece la posición objetivo del enemigo (generalmente el jugador)
    /// @param nuevoObjetivo Posición hacia donde debe moverse el enemigo
    void SetObjective(const std::vector<Player *> newObjectives);

    /// @brief Verifica si el enemigo está vivo
    /// @return true si está vivo, false si está muerto
    bool IsAlive() const { return this->stats.GetHealth() > 0; }

    /*--------------------------*/
    // Botín y Economía
    /*--------------------------*/

    /// @brief Distribuye Pablo Coins a todos los jugadores vivos al morir
    virtual void DropLoot();

    /// @brief Establece la cantidad de Pablo Coins que suelta al morir
    void SetPabloCoinsAtDeath(int cantidad);

    /// @brief Obtiene la cantidad de Pablo Coins que suelta al morir
    int GetPabloCoinsAtDeath() const;

    /*--------------------------*/
    // Estado y Propiedades
    /*--------------------------*/

    /// @brief Obtiene el jugador más cercano al enemigo
    /// @return Puntero al jugador más cercano, nullptr si no hay jugadores
    Player *GetClosestPlayer();

    virtual void CheckCollisions(float deltaTime) override {};

    Stats GetStats() const { return stats; }

    /// @brief Destructor virtual
    virtual ~AEnemy();
};
