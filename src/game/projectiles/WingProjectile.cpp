#include "WingProjectile.hpp"
#include "SpriteSheet.hpp"
#include "SpriteLoaderManager.hpp"
#include <cstdio>
#include <cmath>


WingProjectile::WingProjectile()
{
}

WingProjectile::~WingProjectile()
{
}


void WingProjectile::render()
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(PROJECTILE_TYPE::WING);
    if (sheet.frames.empty()) return;
    animation.frameIndex %= sheet.spriteFrameCount;

    Rectangle src = sheet.frames[animation.frameIndex];

    Vector2 origin = { src.width > 0 ? src.width * 0.5f : -src.width * 0.5f,
                       src.height > 0 ? src.height * 0.5f : -src.height * 0.5f };

    Rectangle dest = { getPosition().x, getPosition().y,
                       src.width, src.height};

    // Rotate according to getDirection()
    Vector2 dir = getDirection();
    float angleDeg = 0.0f;
    if (dir.x != 0.0f || dir.y != 0.0f) {
        angleDeg = atan2f(dir.y, dir.x) * RAD2DEG;
        // Si la punta de la flecha en el sprite apunta hacia ARRIBA,
        // resta 90 grados para que coincida con la dirección.
        angleDeg += 90.0f;
        // Si gira en sentido contrario, prueba angleDeg += 90.0f;
    }

    DrawTexturePro(sheet.texture, src, dest, origin, angleDeg, WHITE);
}
