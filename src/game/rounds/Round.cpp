#include "Round.hpp"
#include <cstdlib>
Round::Round(float duration, float spawnRate, int roundNumber, std::vector<AEnemy*> enemiesToSpawn, std::vector<AEnemy*>& enemiesOnMap)
    : duration(duration), spawnRate(spawnRate), roundNumber(roundNumber), enemiesToSpawn(enemiesToSpawn), enemiesOnMap(enemiesOnMap)
{
}

Round::~Round()
{
    for (AEnemy* enemy : enemiesToSpawn)
    {
        delete enemy;
    }
    // No eliminamos enemiesOnMap porque es una referencia compartida
    // MainGameState se encarga de su limpieza
}

void Round::Update(float deltaTime)
{
    elapsedTime += deltaTime;
    timeSinceLastSpawn += deltaTime;
    if (timeSinceLastSpawn >= spawnRate)
    {
        if (!enemiesToSpawn.empty())
        {
            int randomIndex = rand() % enemiesToSpawn.size();
            AEnemy* enemy = enemiesToSpawn[randomIndex];
            enemiesToSpawn.erase(enemiesToSpawn.begin() + randomIndex);
            enemiesOnMap.push_back(enemy);
        }
        timeSinceLastSpawn = 0.0f;
    }

    for (auto &enemy : enemiesOnMap)
    {
        enemy->Update(deltaTime);
    }

}
void Round::Render()
{
    int screenWidth = GetScreenWidth();
    int fontSize = 30;
    int yOffset = 10;
    
    // Renderizar número de ronda
    std::string roundText = "Round: " + std::to_string(roundNumber);
    int textWidth = MeasureText(roundText.c_str(), fontSize);
    DrawText(roundText.c_str(), (screenWidth - textWidth) / 2, yOffset, fontSize, WHITE);
    
    // Renderizar tiempo restante
    float remainingTime = duration - elapsedTime;
    if (remainingTime < 0) remainingTime = 0;
    int minutes = static_cast<int>(remainingTime) / 60;
    int seconds = static_cast<int>(remainingTime) % 60;
    std::string timeText = "Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    int timeTextWidth = MeasureText(timeText.c_str(), fontSize);
    DrawText(timeText.c_str(), (screenWidth - timeTextWidth) / 2, yOffset + 35, fontSize, YELLOW);
    
    // Renderizar enemigos spawneados + por spawnear
    int totalEnemies = enemiesToSpawn.size() + enemiesOnMap.size();
    std::string enemiesText = "Enemies: " + std::to_string(totalEnemies);
    int enemiesTextWidth = MeasureText(enemiesText.c_str(), fontSize);
    DrawText(enemiesText.c_str(), (screenWidth - enemiesTextWidth) / 2, yOffset + 70, fontSize, RED);
    
    for (auto  &enemy : enemiesOnMap)
    {
        enemy->Render();
    }

}

bool Round::IsRoundOver() const
{
    return elapsedTime >= duration || (enemiesOnMap.empty() && enemiesToSpawn.empty());
}