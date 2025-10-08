#include "AProjectile.hpp"

AProjectile::AProjectile(Vector2 pos, Stats stats)
    : position(pos), stats(stats), active(false)
{
}

AProjectile::~AProjectile()
{
}

Vector2 AProjectile::getPosition() const
{
    return position;
}

Stats AProjectile::getStats() const
{
    return stats;
}