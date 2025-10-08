#pragma once
#include <Stats.hpp>
#include <string>
extern "C"
{
#include <raylib.h>
}

class AProjectile
{
private:
    Stats stats;
    Vector2 position;
    bool active = false;

public:
    AProjectile() = default;
    ~AProjectile();

    virtual void update(float deltaTime) = 0;
    Vector2 getPosition() const;
    Stats getStats() const;
    bool isActive() const;
    void deactivate();
    void activate(Vector2 pos, const Stats &stats);
};