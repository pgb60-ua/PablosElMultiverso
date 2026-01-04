#pragma once
#include "AWeapon.hpp"

class AMeleeWeapon : public AWeapon {

protected:
    /// @brief Actualiza el intervalo de ataque basado en la velocidad de ataque
    float attackInterval = 0.0f;
    /// @brief Velocidad de ataque mínima
    static constexpr float MIN_ATTACK_SPEED = 0.1f;
    /// @brief tiempo desde el último ataque
    float timeSinceLastAttack = 0.0f;

    void UpdateAttackInterval();

public:
    /// @brief Constructor de la clase AMeleeWeapon
    AMeleeWeapon(const std::string &name, const std::string &description,
                 const Stats &stats, ItemRarity itemRarity, int level,
                 const Vector2 &position, std::vector<AEnemy *> &enemiesInRange,
                 std::vector<AEnemy *> &allEnemies);

    virtual ~AMeleeWeapon();

    /// @brief Método para atacar
    virtual void Attack() override = 0;
    
    /// @brief Sobrecarga del ataque melee con deltaTime
    virtual void Attack(float deltaTime);

    void SetStats(const Stats &newStats);

    float GetAttackInterval() const { return attackInterval; }

    void update(float deltaTime, const Vector2 &position) override;
};