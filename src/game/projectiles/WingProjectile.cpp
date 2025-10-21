#include "WingProjectile.hpp"
#include <cstdio>

WingProjectile::WingProjectile()
{
}

WingProjectile::~WingProjectile()
{
}

void WingProjectile::render() const
{
    printf("Rendering WingProjectile at position (%f, %f)\n", getPosition().x, getPosition().y);
    
}
