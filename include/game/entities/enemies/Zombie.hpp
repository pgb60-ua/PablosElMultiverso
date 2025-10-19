#pragma once

#include "AEnemy.hpp"
extern "C"
{
#include <raylib.h>
}

/// @brief Clase que representa un enemigo de tipo Zombie
/// El zombie es un enemigo básico que se mueve lentamente hacia el jugador
/// y realiza ataques cuerpo a cuerpo
class Zombie : public AEnemy
{
private:
public:
    /// @brief Constructor del Zombie
    /// @param hitbox Hitbox del zombie
    /// @param objectives Referencia al vector de jugadores (objetivos)
    /// @param pabloCoinsAtDeath Cantidad de Pablo Coins que suelta al morir
    /// Las estadísticas se cargan automáticamente desde el archivo zombie.json
    Zombie(std::vector<Player *> objectives);

    /*--------------------------*/
    // Métodos Heredados de AEntity
    /*--------------------------*/

    /// @brief Recibe daño y actualiza la salud del zombie
    /// Función utilizada por balas y otras fuentes de daño
    /// @param amount Cantidad de daño recibido
    void TakeDamage(float amount) override;

    /// @brief Realiza un ataque al jugador
    /// @return true si el ataque se realizó, false si está en cooldown
    bool Attack() override;

    /*--------------------------*/
    // Métodos Heredados de AEnemy
    /*--------------------------*/

    /// @brief Mueve el zombie hacia el jugador más cercano
    /// @param deltaTime Tiempo transcurrido desde el último frame
    void Move(float deltaTime) override;

    /// @brief Suelta Pablo Coins al morir
    /// @return Cantidad de Pablo Coins que suelta
    int DropLoot() const override;

    /// @brief Renderiza el zombie
    void Render() override;
    /// @brief Destructor
    ~Zombie() override = default;
};
