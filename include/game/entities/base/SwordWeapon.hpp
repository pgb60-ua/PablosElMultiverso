#pragma once
#include "AMeleeWeapon.hpp"
#include "DataFileManager.hpp"
#include <vector>
#include <memory>

class SwordWeapon : public AMeleeWeapon {
private:
    bool isThrusting = false;
    float thrustTimer = 0.0f;
    float currentThrustAmount = 0.0f;
    
    static constexpr float THRUST_DISTANCE = 40.0f;
    
    static constexpr float DEFAULT_ATTACK_SIZE = 30.0f;
    std::vector<AEnemy*> hitEnemies;

public:
    SwordWeapon(const Vector2& position, std::vector<AEnemy *>& enemiesInRange, std::vector<AEnemy *> &allEnemies);
    virtual ~SwordWeapon();
    
    void Attack() override;
    void Attack(float deltaTime) override;
    void update(float deltaTime, const Vector2 &position) override;
    //void render() override;
};
