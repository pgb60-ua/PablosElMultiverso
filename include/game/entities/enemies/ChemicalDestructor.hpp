#pragma once

#include "AEnemy.hpp"
#include "ChemicalDestructorWeapon.hpp"
#include <vector>

class ChemicalDestructor : public AEnemy
{
private:
    /// @brief Arma del ChemicalDestructor
    std::unique_ptr<ChemicalDestructorWeapon> weapon;

public:
    /// @brief Constructor del ChemicalDestructor
    /// @param players Referencia al vector de jugadores (objetivos)
    ChemicalDestructor(std::vector<Player *> players);

    /// @brief Método de ataque del ChemicalDestructor
    bool Attack() override;

    /// @brief Actualiza el estado del ChemicalDestructor
    void Update(float deltaTime) override;

    /// @brief Renderiza el ChemicalDestructor
    void Render() override;
};
