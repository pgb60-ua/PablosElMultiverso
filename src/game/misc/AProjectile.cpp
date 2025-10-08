#include <AProjectile.hpp>

AProjectile::AProjectile()
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

bool AProjectile::isActive() const
{
    return active;
}

void AProjectile::deactivate()
{
    active = false;
}

void AProjectile::activate(Vector2 pos, Stats stats)
{
    position = pos;
    this->stats = stats;
    active = true;
}