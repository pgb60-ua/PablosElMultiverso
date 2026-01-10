#pragma once
#include "AProjectile.hpp"

class ChemicalDestructorProjectile : public AProjectile
{
public:
    ChemicalDestructorProjectile(std::vector<AEnemy *> &allEnemies);
    ~ChemicalDestructorProjectile();

    void render() override;
};
