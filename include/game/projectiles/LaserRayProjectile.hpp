#pragma once
#include "AProjectile.hpp"

class LaserRayProjectile : public AProjectile
{
private:
    inline static const float TIME_TO_BE_ALIVE = 1.5f; // Segundos que dura el rayo
    inline static const float TIME_DECAY_MULTIPLIER = 2.0f;   // Multiplicador para que el rayo decaiga más rápido
    float timeAlive = 0.0f;

public:
    LaserRayProjectile(std::vector<AEnemy *> &allEnemies);
    void update(float deltaTime) override;
    ~LaserRayProjectile();
    void activate(Vector2 position, Vector2 direction, const Stats &stats) override;
    void updatePositionAndDirection(Vector2 newPosition, Vector2 newDirection); 
    void render() override;
};