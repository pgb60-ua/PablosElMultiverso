#pragma once
#include "AMeleeWeapon.hpp"
#include "DataFileManager.hpp"
#include <memory>

class AxeWeapon : public AMeleeWeapon {
private:
    bool isSwinging = false;
    float swingAngle = 0.0f;
    float swingSpeed = 180.0f; 
    float swingRange = 90.0f; 
    
    std::vector<AEnemy*> hitEnemies;
public:
    /// @brief Constructor de la clase AxeWeapon que carga desde JSON
    AxeWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~AxeWeapon();
    
    /// @brief Método para atacar 
    void Attack() override;

    void Attack(float deltaTime) override;

    void update(float deltaTime, const Vector2 &position) override;

    //void render() override;
};