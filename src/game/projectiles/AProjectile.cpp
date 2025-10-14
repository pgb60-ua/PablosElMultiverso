#include "AProjectile.hpp"

AProjectile::AProjectile()
{
    position = {-110, -110};
    direction = {-110, -110};
    stats = Stats();
    speed = 180.0f;
}

AProjectile::~AProjectile()
{
}

void AProjectile::update(float deltaTime)
{

    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }
}

void AProjectile::deactivate()
{
    active = false;
}

void AProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats)
{
    this->active = true;
    this->position = position;
    this->direction = direction;
    this->stats = stats;
}
