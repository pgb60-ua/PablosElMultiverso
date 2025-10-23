#include "AProjectile.hpp"
#include <iostream>

AProjectile::AProjectile()
{
    shape.type = ShapeType::SHAPE_RECTANGLE;
    setShapePosition(shape, {-110, -110});
    direction = {-110, -110};
    stats = Stats();
    speed = 280.0f;
    enemiesInScene = nullptr;
}

AProjectile::~AProjectile()
{
}

void AProjectile::update(float deltaTime)
{
    if (!active)
        return;

    Vector2 position = getShapePosition(shape);
    position.x += direction.x * speed * deltaTime;
    position.y += direction.y * speed * deltaTime;
    setShapePosition(shape, position);

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }

    if (enemiesInScene != nullptr)
    {
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
}

Vector2 AProjectile::getPosition() const
{
    return getShapePosition(shape);
}
void AProjectile::deactivate()
{
    active = false;
    enemiesInScene = nullptr;
}

void AProjectile::activate(Vector2 position, Vector2 direction, const Stats &stats, const std::vector<AEnemy *> &allEnemies)
{
    this->active = true;
    setShapePosition(shape, position);
    this->direction = direction;
    this->stats = stats;
    this->enemiesInScene = &allEnemies;
}
