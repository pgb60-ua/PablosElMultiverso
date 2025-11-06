#include "InputSystem.hpp"
#include "InputComponent.hpp"
#include "PlayerComponent.hpp"

void InputSystem::Update(entt::registry &registry)
{
    auto view = registry.view<PlayerComponent, InputComponent>();
    for (auto [entity, player, input] : view.each())
    {
        // Intentamos obtener la "fila" asociada a la id y comprobamos que nos haya devuelto algo
        auto it = m_playerBindings.find(player.id);
        if (it == m_playerBindings.end())
        {
            continue; // No hay configuraciones de teclas para este jugador porque no está su index
        }

        // Obtenemos la "lista" de keys
        const KeyBindings &KEYS = it->second;

        // Resetear la dirección
        input.direction = {0, 0};

        // Detectar input de teclas y construir vector de dirección
        if (IsKeyDown(KEYS.up))
        {
            input.direction.y = -1; // Arriba (Y negativa en Raylib)
        }
        if (IsKeyDown(KEYS.down))
        {
            input.direction.y = 1; // Abajo (Y positiva en Raylib)
        }
        if (IsKeyDown(KEYS.left))
        {
            input.direction.x = -1; // Izquierda (X negativa)
        }
        if (IsKeyDown(KEYS.right))
        {
            input.direction.x = 1; // Derecha (X positiva)
        }
    }
}
