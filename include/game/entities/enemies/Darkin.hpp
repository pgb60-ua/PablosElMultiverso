#pragma once

#include "AEnemy.hpp"
#include <vector>

class Darkin : public AEnemy
{
public:
    /// @brief Constructor del Darkin
    /// @param players Referencia al vector de jugadores (objetivos)
    Darkin(std::vector<Player *> players);
};
