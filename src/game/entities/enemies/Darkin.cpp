#include "Darkin.hpp"
#include "AEnemy.hpp"
#include "ScreenConstants.hpp"
#include "Types.hpp"
Darkin::Darkin(std::vector<Player *> players)
    : AEnemy(DataFileManager::GetInstance().GetEnemyStats(ENEMY_TYPE::DARKIN),
             SpriteLoaderManager::GetInstance().GetSpriteHitbox(
                 ENEMY_TYPE::DARKIN,
                 Vector2{(float)(std::rand() % ENEMY_SCREEN_WIDTH), (float)(std::rand() % ENEMY_SCREEN_HEIGHT)}),
             ENEMY_TYPE::DARKIN, players, 15)
{
}
