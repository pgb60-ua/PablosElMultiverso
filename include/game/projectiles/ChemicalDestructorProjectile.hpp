#pragma once
#include "AProjectile.hpp"
#include "Player.hpp"

class ChemicalDestructorProjectile : public AProjectile
{
private:
    std::vector<Player *> players;

public:
    ChemicalDestructorProjectile(std::vector<AEnemy *> &allEnemies, std::vector<Player *> &players);
    ~ChemicalDestructorProjectile();

    void render() override;

    void update(float deltaTime) override;
};
