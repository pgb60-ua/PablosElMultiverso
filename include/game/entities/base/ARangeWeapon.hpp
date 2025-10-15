#pragma once
#include "AWeapon.hpp"
#include "AProjectile.hpp"
#include <vector>
#include <memory>

class ARangeWeapon : public AWeapon {

protected:
    /// @brief Pool de proyectiles
    std::vector<std::unique_ptr<AProjectile>> projectilePool;
    /// @brief Tamaño del pool de proyectiles
    const size_t POOL_SIZE;
    /// @brief Indice que indica en que proyectil del pool estamos
    size_t currentProjectileIndex = 0;

    /// @brief Inicializa el pool de proyectiles
    virtual void InitializeProjectilePool();
    /// @brief Obtiene un proyectil inactivo del pool
    AProjectile* GetProjectileFromPool();
public:
    /// @brief Constructor de la clase RangeWeapon
    ARangeWeapon(const std::string& name, const std::string& description, 
        const Stats& stats, ItemRarity itemRarity, int level, size_t poolSize = 50);
    virtual ~ARangeWeapon();

    /// @brief Método para atacar
    virtual void Attack() override = 0;

    /// @brief Sobrecarga del ataque para armas a distancia
    virtual void Attack(const Vector2& position, const Vector2& direction) = 0;

    /// @brief Método para crear proyectiles
    virtual std::unique_ptr<AProjectile> CreateProjectile() = 0;

    /// @brief Dispara un proyectil desde la posición dada y hacia una dirección dada
    void ShootProjectile(const Vector2& position, const Vector2& direction);

    /// @brief Actualiza todos los proyectiles del pool
    void UpdateProjectiles(float deltaTime);

    /// @brief Getter de los proyectiles activos
    std::vector<AProjectile*> GetActiveProjectiles() const;
    



};
