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
    InitWindow(768, 768, "Pablos El Multiverso");
    InitAudioDevice();

    while (!state_machine.is_game_ending() && !IsKeyDown(KEY_ESCAPE) && !WindowShouldClose())
    {
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();       
    }

    return 0;
}