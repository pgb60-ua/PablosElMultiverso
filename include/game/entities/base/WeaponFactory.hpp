#pragma once
#include "AWeapon.hpp"
#include "Types.hpp"
#include <memory>
#include <vector>

class AEnemy;

class WeaponFactory
{
public:
    /// @brief Crea un arma según el tipo de item
    /// @param itemType El tipo de item (debe ser WEAPON_*)
    /// @param position La posición inicial del arma
    /// @param enemiesInRange Referencia a la lista de enemigos en rango
    /// @param allEnemies Referencia a la lista de todos los enemigos
    /// @return Un unique_ptr al arma creada, o nullptr si el tipo no es válido
    static std::unique_ptr<AWeapon> CreateWeapon(ITEM_TYPE itemType, const Vector2 &position,
                                                 std::vector<AEnemy *> &enemiesInRange,
                                                 std::vector<AEnemy *> &allEnemies, int price = 0);

    /// @brief Crea el arma inicial según el tipo de jugador
    /// @param playerType El tipo de jugador
    /// @param position La posición inicial del arma
    /// @param enemiesInRange Referencia a la lista de enemigos en rango
    /// @param allEnemies Referencia a la lista de todos los enemigos
    /// @param price Precio del arma (por defecto 0 para armas iniciales)
    /// @return Un unique_ptr al arma inicial del jugador
    static std::unique_ptr<AWeapon> CreateStartingWeapon(PLAYER_TYPE playerType, const Vector2 &position,
                                                         std::vector<AEnemy *> &enemiesInRange,
                                                         std::vector<AEnemy *> &allEnemies, int price = 0);

private:
    WeaponFactory() = delete; // Clase con solo métodos estáticos
};
