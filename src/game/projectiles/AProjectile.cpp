#include "AProjectile.hpp"

AProjectile::AProjectile()
{
    shape.type = ShapeType::SHAPE_RECTANGLE;
    setShapePosition(shape, {-110, -110});
    direction = {-110, -110};
    stats = Stats();
}

AProjectile::~AProjectile()
{
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

    for (auto &enemy : *enemiesInScene)
    {
        if (enemy->IsAlive() && CheckCollisionRecs(shape.data.rectangle, enemy->GetHitbox().data.rectangle))
        {
            // std::cout << "Projectile hit an enemy!" << std::endl;
            enemy->TakeDamage(stats.GetPhysicalDamage());
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

void AProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats, const std::vector<AEnemy *> &allEnemies)
{
    this->active = true;
    setShapePosition(shape, position);
    this->direction = direction;
    this->stats = stats;
    this->enemiesInScene = &allEnemies;
}
