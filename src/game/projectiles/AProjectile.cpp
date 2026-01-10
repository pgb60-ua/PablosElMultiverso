#include "AProjectile.hpp"
#include <algorithm>

AProjectile::AProjectile(std::vector<AEnemy *> &allEnemies)
    : enemiesInScene(allEnemies)
{
    shape.type = ShapeType::SHAPE_CIRCLE;
    shape.data.circle.radius = 5.0f; // Radio por defecto
    setShapePosition(shape, {-110, -110});
    direction = {-110, -110};
    stats = Stats();
}

void AProjectile::setRadius(float radius)
{
    if (shape.type == ShapeType::SHAPE_CIRCLE)
    {
        shape.data.circle.radius = radius;
    }
}

void AProjectile::update(float deltaTime)
{
    if (!active)
        return;

    Vector2 position = getShapePosition(shape);
    position.x += direction.x * stats.GetMovementSpeed() * deltaTime;
    position.y += direction.y * stats.GetMovementSpeed() * deltaTime;
    setShapePosition(shape, position);

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }

    for (auto &enemy : enemiesInScene)
    {
        if (enemy->IsAlive() && checkCollisionShapes(shape, enemy->GetHitbox()))
        {
            enemy->TakeDamage(stats);
            if (!enemy->IsAlive())
            {
                delete enemy;
                enemiesInScene.erase(std::find(enemiesInScene.begin(), enemiesInScene.end(), enemy));
            }
            deactivate();
            break;
        }
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
