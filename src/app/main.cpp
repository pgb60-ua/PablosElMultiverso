#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <chrono>
#include <memory>
extern "C"
{
#include <raylib.h>
}

int main()
{
    // Obtener el monitor actual ANTES de crear la ventana
    int currentMonitor = GetCurrentMonitor();

    // Obtener dimensiones del monitor
    int monitorAncho = GetMonitorWidth(currentMonitor);
    int monitorAlto = GetMonitorHeight(currentMonitor);

    // Configurar flags ANTES de InitWindow
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); // Sin bordes ni barra de título

    // Crear ventana con el tamaño del monitor
    InitWindow(monitorAncho, monitorAlto, "Pablos, El Multivero");

    // Mover al monitor actual y posicionar en (0,0)
    SetWindowMonitor(currentMonitor);
    SetWindowPosition(0, 0);

    SetTargetFPS(60);

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);
    SetTargetFPS(120);
    InitAudioDevice();

    while (!state_machine.is_game_ending() && !IsKeyDown(KEY_ESCAPE) && !WindowShouldClose())
    {
        delta_time = GetFrameTime();
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();
    }

    return 0;
}
