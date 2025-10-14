#include "AProjectile.hpp"

class WingProjectile : public AProjectile
{
private:
public:
    WingProjectile();
    ~WingProjectile();
    void render() const override;
};