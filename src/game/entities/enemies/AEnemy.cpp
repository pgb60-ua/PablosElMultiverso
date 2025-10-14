#include "AEnemy.hpp"

AEnemy::AEnemy(Stats stats, const Shape &hitbox, std::vector<Player *> objectives, int pabloCoinsAtDeath)
    : AEntity(stats, hitbox), objectives(objectives), pabloCoinsAtDeath(pabloCoinsAtDeath)
{
}

void AEnemy::SetObjective(const std::vector<Player *> newObjectives) { objectives = newObjectives; }

void AEnemy::SetPabloCoinsAtDeath(int cantidad) { pabloCoinsAtDeath = cantidad; }

int AEnemy::GetPabloCoinsAtDeath() const { return pabloCoinsAtDeath; }
