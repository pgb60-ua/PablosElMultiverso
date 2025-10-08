#include "AEnemy.hpp"

AEnemy::AEnemy(Stats stats, const Shape &hitbox, std::vector<Texture2D *> textures, int pabloCoinsAtDeath, std::vector<Player> &objectives)
    : AEntity(stats, hitbox, textures), objectives(objectives), pabloCoinsAtDeath(pabloCoinsAtDeath)
{
}

void AEnemy::SetObjective(const std::vector<Player> &newObjectives)
{
    objectives = newObjectives;
}

std::vector<Player> AEnemy::GetObjective() const
{
    return objectives;
}

void AEnemy::SetPabloCoinsAtDeath(int cantidad)
{
    pabloCoinsAtDeath = cantidad;
}

int AEnemy::GetPabloCoinsAtDeath() const
{
    return pabloCoinsAtDeath;
}
