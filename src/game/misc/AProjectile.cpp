#include "AProjectile.hpp"

Vector2 AProjectile::getPosition() const
{
    return position;
}

Stats AProjectile::getStats() const
{
    return stats;
}

bool AProjectile::isActive() const
{
    return active;
}

void AProjectile::deactivate()
{
    active = false;
}

void AProjectile::activate(Vector2 position, const Stats &stats)
{
    this->position = position;
    this->stats = stats;
    active = true;
}