#include "Player.hpp"
#include "Types.hpp"
#include "raylib.h"
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

    for (const auto &weapon : weapons)
    {
        // Asumimos que el arma sigue al primer jugador
        weapon->update(deltaTime, Vector2{GetPosition().x + 80, GetPosition().y + 80});
    }
}

void Player::HandleInput(Vector2 newInputDirection)
{
    // Guardar la dirección del input para usarla en Update
    inputDirection = newInputDirection;
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

void Player::AddWeapon(std::unique_ptr<AWeapon> newWeapon)
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
        weapons[index]->Upgrade(newWeapon->GetStats().GetOffensiveStats());
    }
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
