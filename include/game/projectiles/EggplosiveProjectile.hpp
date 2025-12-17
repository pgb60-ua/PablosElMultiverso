#pragma once
#include "AProjectile.hpp"

class EggplosiveProjectile : public AProjectile
{
private:
    inline static const float TIME_TO_BE_ALIVE = 1.5f; 
    bool hasExploded = false;
    float timeAlive = 0.0f;

public:
    EggplosiveProjectile(std::vector<AEnemy *> &allEnemies);
    ~EggplosiveProjectile();

    void update(float deltaTime) override;
    void render() override;
    void activate(Vector2 position, Vector2 direction, const Stats &stats) override;
    
private:
    /// @brief Causa daño en área a todos los enemigos dentro del radio de explosión
    void Explode(float deltaTime);
    void setRadiusDefault();
};
