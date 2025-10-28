#pragma once

#include "AEntity.hpp"
#include "Player.hpp"
#include "SpriteAnimation.hpp"
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

    /// @brief Constructor protegido para clases derivadas
    AEnemy(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath);
    /// @brief Actualiza la animación del enemigo
    void UpdateEnemyAnimation(float deltaTime, ENEMY_TYPE enemyType);

public:
    /*--------------------------*/
    // Movimiento e IA
    /*--------------------------*/

    /// @brief Actualiza el estado del enemigo (animación, cooldowns, etc.)
    virtual void Update(float deltaTime) = 0;

    /// @brief Mueve el enemigo hacia su objetivo
    /// @param deltaTime Tiempo transcurrido desde el último frame
    virtual void Move(float deltaTime) = 0;

    /// @brief Establece la posición objetivo del enemigo (generalmente el jugador)
    /// @param nuevoObjetivo Posición hacia donde debe moverse el enemigo
    void SetObjective(const std::vector<Player *> newObjectives);

    /// @brief Verifica si el enemigo está vivo
    /// @return true si está vivo, false si está muerto
    bool IsAlive() const { return this->stats.GetHealth() > 0; }

    /*--------------------------*/
    // Botín y Economía
    /*--------------------------*/

    /// @brief Suelta Pablo Coins al morir
    /// @return Cantidad de Pablo Coins que suelta
    virtual int DropLoot() { return pabloCoinsAtDeath; };

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

    Stats GetStats() const { return stats; }

    /// @brief Destructor virtual
    virtual ~AEnemy() = default;

    virtual void CheckCollisions(float deltaTime) override;
};
