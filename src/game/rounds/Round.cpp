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
        timeSinceLastSpawn -= spawnRate;
    }

    for (auto &enemy : enemiesOnMap)
    {
        enemy->Update(deltaTime);
    }

}
void Round::Render()
{
    int screenWidth = GetScreenWidth();
    int fontSize = 20;
    int yOffset = 10;
    int spacing = 15; // Espacio entre textos
    
    // Calcular textos
    float remainingTime = duration - elapsedTime;
    if (remainingTime < 0) remainingTime = 0;
    int minutes = static_cast<int>(remainingTime) / 60;
    int seconds = static_cast<int>(remainingTime) % 60;
    int totalEnemies = enemiesToSpawn.size() + enemiesOnMap.size();
    
    std::string roundText = "Round: " + std::to_string(roundNumber);
    std::string timeText = "Time: " + std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
    std::string enemiesText = "Enemies: " + std::to_string(totalEnemies);
    
    // Calcular anchos
    int roundWidth = MeasureText(roundText.c_str(), fontSize);
    int timeWidth = MeasureText(timeText.c_str(), fontSize);
    int enemiesWidth = MeasureText(enemiesText.c_str(), fontSize);
    int totalWidth = roundWidth + spacing + timeWidth + spacing + enemiesWidth;
    
    // Posición inicial centrada
    int startX = (screenWidth - totalWidth) / 2;
    
    // Dibujar cada texto con su color
    DrawText(roundText.c_str(), startX, yOffset, fontSize, WHITE);
    DrawText(timeText.c_str(), startX + roundWidth + spacing, yOffset, fontSize, YELLOW);
    DrawText(enemiesText.c_str(), startX + roundWidth + spacing + timeWidth + spacing, yOffset, fontSize, RED);
    
    for (auto  &enemy : enemiesOnMap)
    {
        enemy->Render();
    }

}

bool Round::IsRoundOver() const
{
    return elapsedTime >= duration || (enemiesOnMap.empty() && enemiesToSpawn.empty());
}