#include "Player.hpp"
#include "raylib.h"
#include <cstdlib>

Player::Player(PLAYER player, Vector2 position)
    : AEntity(DataFileManager::GetInstance().GetPlayerStats(player),
              SpriteLoaderManager::GetSpriteHitbox(player, position))
{
}

void Player::SetOffensiveStatsWithModifiers(const OffensiveStats &itemStats)
{
    OffensiveStats newOffensiveStats = {GetPhysicalDamage() + (itemStats.physicalDamage * GetPhysicalDamageModifier()),
                                        GetMagicDamage() + (itemStats.magicDamage * GetMagicDamageModifier()),
                                        GetAttackSpeed() + (itemStats.attackSpeed * GetAttackSpeedModifier()),
                                        GetCriticalChance() + (itemStats.criticalChance * GetCriticalChanceModifier()),
                                        GetCriticalDamage() + (itemStats.criticalDamage * GetCriticalDamageModifier()),
                                        GetLifeSteal() + (itemStats.lifeSteal * GetLifeStealModifier())};
    stats.SetOffensiveStats(newOffensiveStats);
}

void Player::SetDefensiveStatsWithModifiers(const DefensiveStats &itemStats)
{
    DefensiveStats newDefensiveStats = {GetHealth() + (itemStats.health * GetHealthModifier()),
                                        stats.GetMaxHealth() + (itemStats.healthMax * GetHealthModifier()),
                                        GetMovementSpeed() + (itemStats.movementSpeed * GetMovementSpeedModifier()),
                                        GetAgility() + (itemStats.agility * GetAgilityModifier()),
                                        GetArmor() + (itemStats.armor * GetArmorModifier()),
                                        GetResistance() + (itemStats.resistance * GetResistanceModifier()),
                                        GetHealthRegeneration() +
                                            (itemStats.healthRegeneration * GetHealthRegenerationModifier())};
    stats.SetDefensiveStats(newDefensiveStats);
}

void Player::Move(Vector2 newDirection, float deltaTime)
{
    float speed = GetMovementSpeed();
    Vector2 position = GetPosition();
    Setposition(
        Vector2{position.x + newDirection.x * deltaTime * speed, position.y + newDirection.y * deltaTime * speed});
}

void Player::Update(float deltaTime)
{
    // Actualizar movimiento basado en el input guardado
    if (inputDirection.x != 0 || inputDirection.y != 0)
    {
        Move(inputDirection, deltaTime);
    }
}

void Player::HandleInput(Vector2 newInputDirection)
{
    // Guardar la dirección del input para usarla en Update
    inputDirection = newInputDirection;
}

void Player::TakeDamage(float amount)
{
    float currentHealth = GetHealth();
    float newHealth = currentHealth - amount;

    // Asegurarse de que la vida no sea negativa
    if (newHealth < 0.0f)
    {
        newHealth = 0.0f;
    }

    stats.SetHealth(newHealth);
}

void Player::AddItem(std::shared_ptr<Item> item)
{
    Stats itemStats = item->GetStats();

    // Añadir las stats del item aplicando los modificadores
    SetOffensiveStatsWithModifiers(itemStats.GetOffensiveStats());
    SetDefensiveStatsWithModifiers(itemStats.GetDefensiveStats());

    // Añadir el item al inventario (shared_ptr se copia, incrementa ref count)
    inventory.push_back(item);
}

void Player::AddWeapon(std::unique_ptr<Item> newWeapon)
{
    // Si tengo menos de 4
    if (weapons.size() < WEAPON_MAX)
    {
        weapons.push_back(std::move(newWeapon)); // std::move transfiere ownership
    }
    // Si tengo 4, sumo las stats
    else
    {
        int index = std::rand() % 4;
        weapons[index].Upgrade(newWeapon);
    }
}

Player::~Player()
{
    inventory.clear();
    weapons.clear();
}
