#include <StateMachine.hpp>
#include <MainGameState.hpp>
#include <memory>
#include <chrono>
extern "C" {
    #include <raylib.h>
}

int main()
{ 
    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainGameState>(), false);
    state_machine.handle_state_changes(delta_time);
    int screenWidth  = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);
    InitWindow(screenWidth, screenHeight, "Pablos El Multiverso");
    ToggleFullscreen();
    InitAudioDevice();

    while (!state_machine.is_game_ending() && IsKeyPressed(KEY_SPACE) == false && !WindowShouldClose())
    {
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();       
    }

    return 0;
}