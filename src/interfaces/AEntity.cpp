#include "AEntity.hpp"

AEntity::AEntity(Stats stats, const Shape &hitbox,
                 std::vector<Texture2D *> textures)
    : stats(std::move(stats)), hitbox(hitbox), textures(textures) {}

void AEntity::Render() {}
