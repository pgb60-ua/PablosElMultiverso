#pragma once
#include "AProjectile.hpp"

class LaserRayProjectile : public AProjectile
{
private:
    float timeAlive = 0.0f;

public:
    LaserRayProjectile(std::vector<AEnemy *> &allEnemies);
    void update(float deltaTime) override;
    ~LaserRayProjectile();
    void render() override;
};