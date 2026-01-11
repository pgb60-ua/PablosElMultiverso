#include "Player.hpp"
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
    SetPosition(
        Vector2{position.x + newDirection.x * deltaTime * speed, position.y + newDirection.y * deltaTime * speed});
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
    int newWeaponPrice = newWeapon->GetPrice(); // Obtener precio antes de que se destruya
    int newWeaponLevel = newWeapon->GetLevel();

    for (auto &weapon : weapons)
    {
        if (weapon->GetWeaponType() == newWeaponType && weapon->GetLevel() == newWeaponLevel &&
            weapon->GetLevel() < weapon->GetMaxLevel())
        {
            // Acumular el precio del arma comprada al precio del arma existente
            weapon->Upgrade(newWeapon->GetStats().GetOffensiveStats(), newWeaponPrice);
            break;
        }
    }

    // Si no se pudo mejorar ninguna arma del mismo tipo, no hacer nada
    // (esto no debería suceder si CanAcceptWeapon se usa correctamente)
}

bool Player::CanFuse(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < weapons.size())
    {
        WEAPON_TYPE indexType = weapons[index]->GetWeaponType();
        int indexLevel = weapons[index]->GetLevel();
        if (indexLevel >= weapons[index]->GetMaxLevel())
        {
            return false;
        }
        for (int i = 0; static_cast<size_t>(i) < weapons.size(); i++)
        {
            if (i == index)
            {
                continue;
            }
            if (weapons[i]->GetWeaponType() == indexType && weapons[i]->GetLevel() == indexLevel)
            {
                return true;
            }
        }
    }
    return false;
}

void Player::UpgradeWeapon(int index)
{
    if (index >= 0 && static_cast<size_t>(index) < weapons.size())
    {
        const auto &weapon = weapons[index];
        int weaponToErase = -1;
        WEAPON_TYPE indexType = weapon->GetWeaponType();
        int indexLevel = weapon->GetLevel();
        // Refactorizar
        for (int i = 0; static_cast<size_t>(i) < weapons.size(); i++)
        {
            if (i == index)
            {
                continue;
            }
            if (weapons[i]->GetWeaponType() == indexType && weapons[i]->GetLevel() == indexLevel)
            {
                weaponToErase = i;
                break;
            }
        }
        if (weaponToErase != -1)
        {
            weapon->Upgrade(weapons[weaponToErase]->GetStats().GetOffensiveStats(), weapons[weaponToErase]->GetPrice());
            RemoveWeapon(weaponToErase);
        }
    }
}
