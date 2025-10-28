#pragma once

#include "AProjectile.hpp"

class WingProjectile : public AProjectile
{
private:
public:
    WingProjectile();
    ~WingProjectile();
    void render() override;
};