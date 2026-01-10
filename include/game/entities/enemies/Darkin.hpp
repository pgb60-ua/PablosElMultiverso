#pragma once

#include "AEnemy.hpp"
#include <vector>

class Darkin : public AEnemy
{
public:
    /// @brief Constructor del Darkin
    /// @param players Referencia al vector de jugadores (objetivos)
    Darkin(std::vector<Player *> players);

    /// @brief Actualiza el estado del Darkin
    void Update(float deltaTime) override;

    /// @brief Renderiza el Darkin
    void Render() override;
};
