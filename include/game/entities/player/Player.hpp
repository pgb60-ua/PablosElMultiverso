#pragma once
#include "AEntity.hpp"
#include "AWeapon.hpp"
#include "DataFileManager.hpp"
#include "Item.hpp"
#include "SpriteLoaderManager.hpp"
#include "Stats.hpp"
#include "Types.hpp"
#include "raylib.h"
#include <memory>
#include <vector>

// Clase que representa el player
class Player : public AEntity
{
private:
    void ImportModifiers(PLAYER_TYPE player);
    static constexpr float BASE_MULTIPLIER = 1.0f;
    std::vector<std::shared_ptr<Item>> inventory;
    std::vector<std::unique_ptr<AWeapon>> weapons;
    static constexpr int WEAPON_MAX = 4;
    Vector2 inputDirection{0, 0};
    PLAYER_TYPE player;

protected:
    /// @brief Modificador multiplicativo de vida
    float healthModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de velocidad de movimiento
    float movementSpeedModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de agilidad
    float agilityModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de velocidad de ataque
    float attackSpeedModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de daño físico
    float physicalDamageModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de daño mágico
    float magicDamageModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de armadura
    float armorModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de resistencia mágica
    float resistanceModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de probabilidad de crítico
    float criticalChanceModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de daño crítico
    float criticalDamageModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de robo de vida
    float lifeStealModifier = BASE_MULTIPLIER;

    /// @brief Modificador multiplicativo de regeneración de vida
    float healthRegenerationModifier = BASE_MULTIPLIER;

public:
    Player(PLAYER_TYPE player, Vector2 position);
    // Getters de stats
    /// @brief Obtiene los puntos de vida actuales
    float GetHealth() const { return stats.GetHealth(); }

    /// @brief Obtiene la velocidad de movimiento actual
    float GetMovementSpeed() const { return stats.GetMovementSpeed(); }

    /// @brief Obtiene la agilidad actual
    float GetAgility() const { return stats.GetAgility(); }

    /// @brief Obtiene la velocidad de ataque actual
    float GetAttackSpeed() const { return stats.GetAttackSpeed(); }

    /// @brief Obtiene el daño físico actual
    float GetPhysicalDamage() const { return stats.GetPhysicalDamage(); }

    /// @brief Obtiene el daño mágico actual
    float GetMagicDamage() const { return stats.GetMagicDamage(); }

    /// @brief Obtiene la armadura actual
    float GetArmor() const { return stats.GetArmor(); }

    /// @brief Obtiene la resistencia mágica actual
    float GetResistance() const { return stats.GetResistance(); }

    /// @brief Obtiene la probabilidad de crítico actual
    float GetCriticalChance() const { return stats.GetCriticalChance(); }

    /// @brief Obtiene el multiplicador de daño crítico actual
    float GetCriticalDamage() const { return stats.GetCriticalDamage(); }

    /// @brief Obtiene el porcentaje de robo de vida actual
    float GetLifeSteal() const { return stats.GetLifeSteal(); }

    /// @brief Obtiene la regeneración de vida por segundo actual
    float GetHealthRegeneration() const { return stats.GetHealthRegeneration(); }

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
    float GetHealthRegenerationModifier() const { return healthRegenerationModifier; }

    /**/
    // Setters de stats (aplican modificador automáticamente)

    /// @brief Establece los puntos de vida base y aplica modificador
    void SetHealth(float newHealth) { stats.SetHealth(newHealth * healthModifier); }

    /// @brief Establece la velocidad de movimiento base y aplica modificador
    void SetMovementSpeed(float newSpeed) { stats.SetMovementSpeed(newSpeed * movementSpeedModifier); }

    /// @brief Establece la agilidad base y aplica modificador
    void SetAgility(float newAgility) { stats.SetAgility(newAgility * agilityModifier); }

    /// @brief Establece la velocidad de ataque base y aplica modificador
    void SetAttackSpeed(float newSpeed) { stats.SetAttackSpeed(newSpeed * attackSpeedModifier); }

    /// @brief Establece el daño físico base y aplica modificador
    void SetPhysicalDamage(float newDamage) { stats.SetPhysicalDamage(newDamage * physicalDamageModifier); }

    /// @brief Establece el daño mágico base y aplica modificador
    void SetMagicDamage(float newDamage) { stats.SetMagicDamage(newDamage * magicDamageModifier); }

    /// @brief Establece la armadura base y aplica modificador
    void SetArmor(float newArmor) { stats.SetArmor(newArmor * armorModifier); }

    /// @brief Establece la resistencia mágica base y aplica modificador
    void SetResistance(float newResistance) { stats.SetResistance(newResistance * resistanceModifier); }

    /// @brief Establece la probabilidad de crítico base y aplica modificador
    void SetCriticalChance(float newChance) { stats.SetCriticalChance(newChance * criticalChanceModifier); }

    /// @brief Establece el multiplicador de daño crítico base y aplica modificador
    void SetCriticalDamage(float newMultiplier) { stats.SetCriticalDamage(newMultiplier * criticalDamageModifier); }

    /// @brief Establece el porcentaje de robo de vida base y aplica modificador
    void SetLifeSteal(float newLifeSteal) { stats.SetLifeSteal(newLifeSteal * lifeStealModifier); }

    /// @brief Establece la regeneración de vida base y aplica modificador
    void SetHealthRegeneration(float newRegeneration)
    {
        stats.SetHealthRegeneration(newRegeneration * healthRegenerationModifier);
    }

    /*--------------------------*/
    // Setters de modificadores
    /*--------------------------*/

    /// @brief Establece el modificador de vida
    void SetHealthModifier(float newModifier) { healthModifier = newModifier; }

    /// @brief Establece el modificador de velocidad de movimiento
    void SetMovementSpeedModifier(float newModifier) { movementSpeedModifier = newModifier; }

    /// @brief Establece el modificador de agilidad
    void SetAgilityModifier(float newModifier) { agilityModifier = newModifier; }

    /// @brief Establece el modificador de velocidad de ataque
    void SetAttackSpeedModifier(float newModifier) { attackSpeedModifier = newModifier; }

    /// @brief Establece el modificador de daño físico
    void SetPhysicalDamageModifier(float newModifier) { physicalDamageModifier = newModifier; }

    /// @brief Establece el modificador de daño mágico
    void SetMagicDamageModifier(float newModifier) { magicDamageModifier = newModifier; }

    /// @brief Establece el modificador de armadura
    void SetArmorModifier(float newModifier) { armorModifier = newModifier; }

    /// @brief Establece el modificador de resistencia mágica
    void SetResistanceModifier(float newModifier) { resistanceModifier = newModifier; }

    /// @brief Establece el modificador de probabilidad de crítico
    void SetCriticalChanceModifier(float newModifier) { criticalChanceModifier = newModifier; }

    /// @brief Establece el modificador de daño crítico
    void SetCriticalDamageModifier(float newModifier) { criticalDamageModifier = newModifier; }

    /// @brief Establece el modificador de robo de vida
    void SetLifeStealModifier(float newModifier) { lifeStealModifier = newModifier; }

    /// @brief Establece el modificador de regeneración de vida
    void SetHealthRegenerationModifier(float newModifier) { healthRegenerationModifier = newModifier; }

    void SetOffensiveStatsWithModifiers(const OffensiveStats &offensiveStats);
    void SetDefensiveStatsWithModifiers(const DefensiveStats &defensiveStats);

    PLAYER_TYPE GetPlayerType() { return player; }
    void SetPlayerType(PLAYER_TYPE player) { this->player = player; }

    void Move(Vector2 newDirection, float deltaTime);
    void TakeDamage(float amount) override;
    void Update(float deltaTime) override;
    void HandleInput(Vector2 inputDirection);
    void AddItem(std::shared_ptr<Item> item);
    void AddWeapon(std::unique_ptr<AWeapon> newWeapon);
    void Render() override;
    bool Attack() override;

    ~Player();
};
