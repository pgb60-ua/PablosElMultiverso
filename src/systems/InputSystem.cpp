#include "InputSystem.hpp"
#include "EntityComponent.hpp"
#include "InputComponent.hpp"
#include "RenderComponents.hpp"

void InputSystem::Update(entt::registry &registry)
{
    // Ya que no quiero almacenar id en el componente player, asi puedo usar
    size_t playerId = 0;
    auto view = registry.view<InputComponent, RenderEntityComponent, const EntityComponent>();
    for (auto [entity, input, render, entComponent] : view.each())
    {
        playerId++;
        // Si el player esta muerto o en la tienda no tiene que moverse porque no se renderiza
        // TODO : Cuando está en la tienda tiene las teclas para comprar en vez de para mover
        if (entComponent.state == DEAD || entComponent.state == SHOP)
            continue;
        // Intentamos obtener la "fila" asociada a la id y comprobamos que nos haya devuelto algo
        const auto it = m_playerBindings.find(playerId);
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
        if (input.direction.x != 0)
        {
            render.animation.flipped = (input.direction.x < 0);
        }
    }
}
