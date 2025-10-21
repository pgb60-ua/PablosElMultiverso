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
    /// @brief Registra todas las instancias activas para aplicar las reglas de Boids
    static std::vector<Zombie *> s_allZombies;

    /// @brief Velocidad actual acumulada del zombie para suavizar el movimiento
    Vector2 velocity;

    /// @brief Radio de percepción para detectar otros zombies cercanos
    static constexpr float PERCEPTION_RADIUS = 250.0f;
    /// @brief Radio de separación para evitar aglomeración con otros zombies
    static constexpr float SEPARATION_RADIUS = 80.0f;
    /// @brief Peso de la regla de alineación (seguir dirección del grupo)
    static constexpr float ALIGNMENT_WEIGHT = 0.5f;
    /// @brief Peso de la regla de cohesión (moverse hacia el centro del grupo)
    static constexpr float COHESION_WEIGHT = 0.45f;
    /// @brief Peso de la regla de separación (evitar colisiones con otros zombies)
    static constexpr float SEPARATION_WEIGHT = 0.75f;
    /// @brief Peso de atracción hacia el objetivo (jugador)
    static constexpr float TARGET_WEIGHT = 1.2f;
    /// @brief Multiplicador máximo de fuerza para limitar aceleraciones abruptas
    static constexpr float MAX_FORCE_MULTIPLIER = 1.6f;

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

    /// @brief Renderiza el zombie
    void Render() override;
    /// @brief Destructor
    ~Zombie() override;
};
