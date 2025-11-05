#pragma once
#include "AProjectile.hpp"

class SniperProjectile : public AProjectile
{
public:
    SniperProjectile(std::vector<AEnemy *> &allEnemies);
    ~SniperProjectile();

    void render() override;
};