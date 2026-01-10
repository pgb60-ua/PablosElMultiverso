#include "ChemicalDestructorProjectile.hpp"
#include <SpriteLoaderManager.hpp>
#include <SpriteSheet.hpp>
#include <cmath>
#include <iostream>

ChemicalDestructorProjectile::ChemicalDestructorProjectile(std::vector<AEnemy *> &allEnemies,
                                                           std::vector<Player *> &players)
    : AProjectile(allEnemies), players(players)
{
    // Calcular el radio basado en el sprite del proyectil Sniper
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::CHEMICAL_BULLET);
    if (!sheet.frames.empty())
    {
        const Rectangle &frame = sheet.frames[0];
        // El radio es la mitad del lado más pequeño del sprite
        float width = std::abs(frame.width);
        float height = std::abs(frame.height);
        float radius = std::min(width, height) * 0.5f;
        setRadius(radius);
    }
}

ChemicalDestructorProjectile::~ChemicalDestructorProjectile() {}

void ChemicalDestructorProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::CHEMICAL_BULLET);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

    Rectangle dest = {getPosition().x, getPosition().y, src.width, src.height};

    // Rotate according to getDirection()
    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f)
    {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
        angleDeg += 90.0f;
    }

    DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
}

void ChemicalDestructorProjectile::update(float deltaTime)
{
    if (!active)
        return;
    std::cout << "Updating ChemicalDestructorProjectile" << std::endl;
    Vector2 position = getShapePosition(shape);
    position.x += direction.x * stats.GetMovementSpeed() * deltaTime;
    position.y += direction.y * stats.GetMovementSpeed() * deltaTime;
    setShapePosition(shape, position);

    if (position.x < 0 || position.x > GetScreenWidth() || position.y < 0 || position.y > GetScreenHeight())
    {
        deactivate();
    }

    // Chequear colisión con players
    for (auto &player : players)
    {
        if (player->IsAlive() && checkCollisionShapes(shape, player->GetHitbox()))
        {
            player->TakeDamage(stats);
            deactivate();
            break;
        }
    }
}
