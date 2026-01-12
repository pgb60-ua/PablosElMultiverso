#include "Player.hpp"
#include "AEnemy.hpp"
#include "AEntity.hpp"
#include "Types.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdlib>
#include <raymath.h>

Player::Player(PLAYER_TYPE player, Vector2 position, std::vector<AEnemy *> &allEnemies)
    : AEntity(DataFileManager::GetInstance().GetPlayerStats(player),
              SpriteLoaderManager::GetInstance().GetSpriteHitbox(player, position)),
      allEnemies(allEnemies)
{
    SetPlayerType(player);
    ImportModifiers(player);
    UpdateEnemiesInRange();
}

void Player::UpdateEnemiesInRange()
{
    enemiesInRange.clear();

    for (AEnemy *enemy : allEnemies)
    {
        Vector2 playerPos = GetPosition();
        Vector2 enemyPos = enemy->GetPosition();
        float distance = Vector2Distance(playerPos, enemyPos);
        if (distance <= DISTANCE_RANGE)
        {
            enemiesInRange.push_back(enemy);
        }
    }
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
    Vector2 newPosition =
        Vector2{position.x + newDirection.x * deltaTime * speed, position.y + newDirection.y * deltaTime * speed};
    if (newPosition.x + hitbox.data.rectangle.width > GetScreenWidth())
    {
        newPosition.x = GetScreenWidth() - hitbox.data.rectangle.width;
    }
    if (newPosition.y + hitbox.data.rectangle.height > GetScreenHeight())
    {
        newPosition.y = GetScreenHeight() - hitbox.data.rectangle.height;
    }
    if (newPosition.x < 0)
    {
        newPosition.x = 0;
    }
    if (newPosition.y < 0)
    {
        newPosition.y = 0;
    }
    SetPosition(newPosition);
}

void Player::Update(float deltaTime)
{
    // Actualizar movimiento basado en el input guardado
    if (inputDirection.x != 0 || inputDirection.y != 0)
    {
        Move(inputDirection, deltaTime);
    }
    UpdateEnemiesInRange();
    UpdatePlayerAnimation(deltaTime);

    // Actualizar posición de las armas usando los offsets almacenados
    Vector2 playerPos = GetPosition();
    playerPos.x += hitbox.data.rectangle.width * 0.5f;
    playerPos.y += hitbox.data.rectangle.height * 0.5f;

    for (size_t i = 0; i < weapons.size(); i++)
    {
        weapons[i]->update(deltaTime, playerPos);
    }

    AEntity::Update(deltaTime);
}

void Player::HandleInput(Vector2 newInputDirection)
{
    // Guardar la dirección del input para usarla en Update
    inputDirection = newInputDirection;
}

void Player::AddItem(const Item *item)
{
    const Stats itemStats = item->GetStats();

    // Añadir las stats del item aplicando los modificadores
    SetOffensiveStatsWithModifiers(itemStats.GetOffensiveStats());
    SetDefensiveStatsWithModifiers(itemStats.GetDefensiveStats());

    inventory.push_back(item);
}

void Player::AddWeapon(std::unique_ptr<AWeapon> newWeapon)
{
    // Si tengo menos de 4
    if (weapons.size() < WEAPON_MAX)
    {
        weapons.push_back(std::move(newWeapon)); // std::move transfiere ownership

        // Redistribuir todas las armas equitativamente en el círculo
        size_t totalWeapons = weapons.size();
        float angleStep = 360.0f / totalWeapons; // Separación uniforme

        for (size_t i = 0; i < totalWeapons; i++)
        {
            float angle = i * angleStep; // 0°, angleStep°, 2*angleStep°, ...
            weapons[i]->SetOrbitAngle(angle);
        }
    }
    // Si tengo 4, buscar un arma del mismo tipo que tenga el mismo nivel
    else
    {
        UpgradeWeapons(std::move(newWeapon));
    }
}

bool Player::CanAcceptWeapon(WEAPON_TYPE weaponType, int weaponLevel) const
{
    // Si tengo menos de 4 armas, siempre puedo aceptar
    if (weapons.size() < WEAPON_MAX)
    {
        return true;
    }

    // Si tengo 4 armas, verificar si hay al menos una del mismo tipo y mismo nivel que el arma entrante
    for (const auto &weapon : weapons)
    {
        // Si un arma es del mismo tipo y mismo nivel y no es nivel maximo
        if (weapon->GetWeaponType() == weaponType && weapon->GetLevel() == weaponLevel &&
            weapon->GetLevel() < weapon->GetMaxLevel())
        {
            return true;
        }
    }

    return false;
}

Player::~Player()
{
    inventory.clear();
    weapons.clear();
}

void Player::ImportModifiers(PLAYER_TYPE player)
{
    const DataMap &data = DataFileManager::GetInstance().GetData(player);

    auto getModifier = [&data](const std::string &key) -> float
    {
        auto it = data.find(key);
        if (it != data.end())
        {
            if (const float *val = std::get_if<float>(&it->second))
            {
                return *val;
            }
            if (const int *val = std::get_if<int>(&it->second))
            {
                return static_cast<float>(*val);
            }
        }
        return BASE_MULTIPLIER; // Default: 1.0f
    };

    // Cargar todos los modificadores desde el JSON
    healthModifier = getModifier("health_modifier");
    movementSpeedModifier = getModifier("movement_speed_modifier");
    agilityModifier = getModifier("agility_modifier");
    attackSpeedModifier = getModifier("attack_speed_modifier");
    physicalDamageModifier = getModifier("physical_damage_modifier");
    magicDamageModifier = getModifier("magic_damage_modifier");
    armorModifier = getModifier("armor_modifier");
    resistanceModifier = getModifier("resistance_modifier");
    criticalChanceModifier = getModifier("critical_chance_modifier");
    criticalDamageModifier = getModifier("critical_damage_modifier");
    lifeStealModifier = getModifier("life_steal_modifier");
    healthRegenerationModifier = getModifier("health_regeneration_modifier");
}

void Player::Render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(player);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];
    if (inputDirection.x != 0)
    {
        animation.flipped = (inputDirection.x < 0);
    }
    if (animation.flipped)
    {
        src.width *= -1.0f;
    }

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

    Rectangle dest = {hitbox.data.rectangle.x + hitbox.data.rectangle.width * 0.5f,
                      hitbox.data.rectangle.y + hitbox.data.rectangle.height * 0.5f, src.width, src.height};

    DrawTexturePro(sheet.texture, src, dest, origin, 0, animation.color);

    // Renderizar armas (la posición ya se actualiza en Update())
    for (const auto &weapon : weapons)
    {
        weapon->render();
    }
}

bool Player::Attack()
{
    // TODO: Implementar lógica de ataque con las armas disponibles
    // Por ahora retorna false indicando que no se realizó ataque
    return false;
}

void Player::UpdatePlayerAnimation(float deltaTime)
{
    animation.timeAccumulator += deltaTime;

    if (animation.timeAccumulator >= animation.FRAME_DURATION)
    {
        animation.timeAccumulator = 0.0f;
        animation.frameIndex++;
        animation.frameIndex %= SpriteLoaderManager::GetInstance().GetSpriteSheet(player).spriteFrameCount;
    }
}

void Player::CheckCollisions(float deltaTime)
{
    if (receiveDamageCooldownTime < COOLDOWN_DAMAGE_TIME)
    {
        receiveDamageCooldownTime += deltaTime;
        return;
    }
    receiveDamageCooldownTime -= COOLDOWN_DAMAGE_TIME;
    animation.color = WHITE;
    for (auto &enemy : enemiesInRange)
    {
        // Obtener las hitboxes
        Shape playerHitbox = GetHitbox();
        Shape enemyHitbox = enemy->GetHitbox();

        // Comprobar colisión entre las hitboxes usando CheckCollisionRecs de raylib
        if (CheckCollisionRecs(playerHitbox.data.rectangle, enemyHitbox.data.rectangle))
        {
            TakeDamage(enemy->GetStats());
            return;
        }
    }
}

int Player::FindMatchingWeapon(WEAPON_TYPE weaponType, int weaponLevel, int excludeIndex) const
{
    for (int i = 0; static_cast<size_t>(i) < weapons.size(); i++)
    {
        if (i == excludeIndex)
        {
            continue;
        }
        if (weapons[i]->GetWeaponType() == weaponType && weapons[i]->GetLevel() == weaponLevel)
        {
            return i;
        }
    }
    return -1;
}

void Player::RemoveWeapon(int index)
{
    if (index < 0 || static_cast<std::size_t>(index) >= weapons.size())
    {
        return; // Índice inválido, no hacer nada
    }

    // Eliminar el arma
    weapons.erase(weapons.begin() + index);

    // Redistribuir ángulos de las armas restantes
    size_t totalWeapons = weapons.size();
    if (totalWeapons > 0)
    {
        float angleStep = 360.0f / totalWeapons;
        for (size_t i = 0; i < totalWeapons; i++)
        {
            float angle = i * angleStep;
            weapons[i]->SetOrbitAngle(angle);
        }
    }
}

void Player::UpgradeWeapons(std::unique_ptr<AWeapon> newWeapon)
{
    WEAPON_TYPE newWeaponType = newWeapon->GetWeaponType();
    int newWeaponPrice = newWeapon->GetPrice();
    int newWeaponLevel = newWeapon->GetLevel();

    int matchingIndex = FindMatchingWeapon(newWeaponType, newWeaponLevel);

    if (matchingIndex != -1 && weapons[matchingIndex]->GetLevel() < weapons[matchingIndex]->GetMaxLevel())
    {
        // Acumular el precio del arma comprada al precio del arma existente
        weapons[matchingIndex]->Upgrade(newWeapon->GetStats().GetOffensiveStats(), newWeaponPrice);
    }

    // Si no se pudo mejorar ninguna arma del mismo tipo, no hacer nada
    // (esto no debería suceder si CanAcceptWeapon se usa correctamente)
}

bool Player::CanFuse(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= weapons.size())
    {
        return false;
    }

    const auto &weapon = weapons[index];
    if (weapon->GetLevel() >= weapon->GetMaxLevel())
    {
        return false;
    }

    return FindMatchingWeapon(weapon->GetWeaponType(), weapon->GetLevel(), index) != -1;
}

void Player::UpgradeWeapon(int index)
{
    if (index < 0 || static_cast<size_t>(index) >= weapons.size())
    {
        return;
    }

    const auto &weapon = weapons[index];
    int matchingIndex = FindMatchingWeapon(weapon->GetWeaponType(), weapon->GetLevel(), index);

    if (matchingIndex != -1)
    {
        weapon->Upgrade(weapons[matchingIndex]->GetStats().GetOffensiveStats(), weapons[matchingIndex]->GetPrice());
        RemoveWeapon(matchingIndex);
    }
}
