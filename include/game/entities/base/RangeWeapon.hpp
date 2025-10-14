#pragma once
#include "Weapon.hpp"
#include "AProjectile.hpp"
#include <vector>
#include <memory>

class RangeWeapon : public Weapon {

protected:
    /// @brief Pool de proyectiles
    std::vector<std::unique_ptr<AProjectile>> projectilePool;
    /// @brief Tamaño del pool de proyectiles
    size_t POOL_SIZE = 50;
    /// @brief Indice que indica en que proyectil del pool estamos
    size_t currentProjectileIndex = 0;

    /// @brief Inicializa el pool de proyectiles
    virtual void InitializeProjectilePool();
    /// @brief Obtiene un proyectil inactivo del pool
    AProjectile* GetProjectileFromPool();
public:
    /// @brief Constructor de la clase RangeWeapon
    RangeWeapon(const std::string& name, const std::string& description, 
        const Stats& stats, ItemRarity itemRarity, int level, size_t poolSize = 50);
    virtual ~RangeWeapon();

    /// @brief Método para atacar
    void Attack() override = 0;

    /// @brief Método para crear proyectiles
    virtual std::unique_ptr<AProjectile> CreateProjectile() = 0;

    /// @brief Dispara un proyectil desde la posición dada y hacia una direcciñon dada
    void ShootProjectile(const Vector2& position, const Vector2& direction);

    /// @brief Actualiza todos los proyectiles del pool
    void UpdateProjectiles(float deltaTime);

    /// @brief Getter de los proyectiles activos
    std::vector<AProjectile*> GetActiveProjectiles() const;
    



};
