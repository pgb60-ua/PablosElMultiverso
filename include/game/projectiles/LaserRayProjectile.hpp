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
    void activate(Vector2 position, Vector2 direction, const Stats &stats) override;
    void render() override;
};