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
    bool active;

public:
    AProjectile(Vector2 pos, Stats stats);
    ~AProjectile();

    void update(float deltaTime);
    Vector2 getPosition() const;
    Stats getStats() const;
    bool isActive() const = 0;
    void deactivate() = 0;
};