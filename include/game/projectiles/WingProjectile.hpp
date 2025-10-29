#pragma once

#include "AProjectile.hpp"

class WingProjectile : public AProjectile
{
private:
public:
    WingProjectile(std::vector<AEnemy *> &allEnemies);
    ~WingProjectile();
    void render() override;
};