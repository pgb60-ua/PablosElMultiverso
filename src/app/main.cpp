#include <MainGameState.hpp>
#include <StateMachine.hpp>
#include <MainMenuState.hpp>
#include <chrono>
#include <memory>
extern "C"
{
#include <raylib.h>
}

int main()
{

    // Configurar flags ANTES de InitWindow
    SetConfigFlags(FLAG_WINDOW_UNDECORATED); // Sin bordes ni barra de título

    int monitorIndex = 0;

    // Obtener dimensiones del monitor
    int monitorAncho = GetMonitorWidth(monitorIndex);
    int monitorAlto = GetMonitorHeight(monitorIndex);

    // Crear ventana con el tamaño del monitor
    InitWindow(monitorAncho, monitorAlto, "Pablos, El Multiverso");

    // Obtener el monitor actual
    int currentMonitor = GetCurrentMonitor();

    // Ahora sí, obtener las dimensiones correctas del monitor actual
    monitorAncho = GetMonitorWidth(currentMonitor);
    monitorAlto = GetMonitorHeight(currentMonitor);

    if (currentMonitor != monitorIndex)
    {
        SetWindowSize(monitorAncho, monitorAlto);
        SetWindowMonitor(currentMonitor);
        SetWindowPosition(0, 0);
    }

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainMenuState>(), false);
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
    CloseAudioDevice();

    return 0;
}
