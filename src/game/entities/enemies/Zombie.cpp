#include "Zombie.hpp"
#include "AEnemy.hpp"
#include "ScreenConstants.hpp"
#include "Types.hpp"
#include <cmath>
#include <raymath.h>

Zombie::Zombie(std::vector<Player *> objectives)
    : AEnemy(DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::ZOMBIE),
             SpriteLoaderManager::GetInstance().GetSpriteHitbox(
                 ENEMY_TYPE::ZOMBIE,
                 Vector2{(float)(std::rand() % ENEMY_SCREEN_WIDTH), (float)(std::rand() % ENEMY_SCREEN_HEIGHT)}),
             ENEMY_TYPE::ZOMBIE, objectives, 10)
{
    // Las stats se cargan automáticamente desde zombie.json en la lista de inicialización
}
