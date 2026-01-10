#include "ChemicalDestructorProjectile.hpp"
#include <SpriteSheet.hpp>
#include <SpriteLoaderManager.hpp>
#include <cmath>

ChemicalDestructorProjectile::ChemicalDestructorProjectile(std::vector<AEnemy *> &allEnemies)
    : AProjectile(allEnemies)
{
    // Calcular el radio basado en el sprite del proyectil Sniper
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::CHEMICAL_DESTRUCTOR);
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

ChemicalDestructorProjectile::~ChemicalDestructorProjectile()
{
}

void ChemicalDestructorProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::CHEMICAL_DESTRUCTOR);
    if (sheet.frames.empty())
        return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = {src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                      src.height > 0 ? src.height * 0.5f : -src.height * 0.5f};

    Rectangle dest = {getPosition().x, getPosition().y,
                      src.width, src.height};

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
