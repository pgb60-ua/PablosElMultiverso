#include "AProjectile.hpp"

AProjectile::AProjectile()
{
    shape.type = ShapeType::SHAPE_RECTANGLE;
    setShapePosition(shape, {-110, -110});
    direction = {-110, -110};
    stats = Stats();
    speed = 180.0f;
}

AProjectile::~AProjectile()
{
}

void AProjectile::update(float deltaTime)
{
    Vector2 position = getShapePosition(shape);
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    setShapePosition(shape, position);

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }
}

Vector2 AProjectile::getPosition() const
{
    return getShapePosition(shape);
}
void AProjectile::deactivate()
{
    active = false;
}

void AProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats)
{
    this->active = true;
    setShapePosition(shape, position);
    this->direction = direction;
    this->stats = stats;
}
