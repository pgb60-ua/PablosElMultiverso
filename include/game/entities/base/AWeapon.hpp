#pragma once
#include "Item.hpp"
#include "DataFileManager.hpp"
#include "AEnemy.hpp"
#include <string>
#include <SpriteAnimation.hpp>

class AEnemy;

/// @brief Helper struct para obtener datos de armas desde JSON
struct WeaponData
{
    std::string name;
    std::string description;
    Stats stats;
    ItemRarity rarity;
    int level;
};

class AWeapon : public Item
{
private:
    /// @brief Máxima probabilidad de crítico
    const float MAX_CRITICAL_CHANCE = 30.0f;
    /// @brief Máximo multiplicador de daño crítico
    const float MAX_CRITICAL_DAMAGE = 10.0f;
    /// @brief Máximo porcentaje de robo de vida
    const float MAX_LIFE_STEAL = 50.0f;
    /// @brief Multiplicador de velocidad de ataque
    const float ATTACK_SPEED_MULTIPLIER = 0.5f;
    /// @brief Multiplicador de daño crítico
    const float CRITICAL_DAMAGE_MULTIPLIER = 0.2f;
    /// @brief Multiplicador de robo de vida
    const float LIFE_STEAL_MULTIPLIER = 0.2f;
    /// @brief Multiplicador de impacto crítico
    const float CRITICAL_CHANCE_MULTIPLIER = 0.3f;
    /// @brief Animación del sprite del arma
    SpriteAnimation animation;

protected:
    /// @brief Tipo de arma
    WEAPON_TYPE weaponType;
    /// @brief Nivel del arma
    int level;
    /// @brief Nivel máximo del arma
    const int MAXLEVEL = 4;
    /// @brief Posición del arma
    Vector2 position;
    /// @brief Dirección del arma
    Vector2 direction;
    /// @brief Enemigos en rango del arma
    std::vector<AEnemy *> &enemiesInRange;
    /// @brief Todos los enemigos del nivel
    std::vector<AEnemy *> &allEnemies;

    /// @brief Obtiene un string del JSON
    static std::string GetStringFromJSON(const std::string &key, WEAPON_TYPE type, const std::string &defaultValue);
    /// @brief Obtiene un int del JSON
    static int GetIntFromJSON(const std::string &key, WEAPON_TYPE type, int defaultValue);
    /// @brief Obtiene la rareza del JSON
    static ItemRarity GetRarityFromJSON(WEAPON_TYPE type);
    /// @brief Calcula la dirección de ataque normalizada
    Vector2 CalculateDirection();

public:
    /// @brief Constructor de la clase Weapon
    AWeapon(const std::string &name, const std::string &description, const Stats &stats, ItemRarity itemRarity, int level,const Vector2 &position, std::vector<AEnemy *> &enemiesInRange, std::vector<AEnemy *> &allEnemies);

    /// @brief Getter del tipo de arma
    WEAPON_TYPE GetWeaponType() const { return weaponType; }
    /// @brief Getter del nivel del arma
    int GetLevel() const { return level; }
    /// @brief Getter del nivel máximo del arma
    int GetMaxLevel() const { return MAXLEVEL; }
    /// @brief Sube de nivel el arma
    bool Upgrade(const OffensiveStats &newOffensiveStats);
    /// @brief Getter de la posición del arma
    Vector2 GetPosition() const { return position; }
    /// @brief Setter de la posición del arma
    void SetPosition(const Vector2 &newPosition) { position = newPosition; }
    /// @brief Setter del tipo de arma
    void SetWeaponType(WEAPON_TYPE newType) { weaponType = newType; }
    /// @brief Setter de la dirección del arma
    void SetDirection(const Vector2 &newDirection) { direction = newDirection; }

    /// @brief Método para ajustar las stats del arma según las stats del jugador
    void SetStatsFromPlayer(const Stats& playerStats);

    /// @brief Método para atacar
    virtual void Attack() = 0;
    virtual void render();
    virtual void update(float deltaTime, const Vector2 &position);
    virtual ~AWeapon() {}
};
