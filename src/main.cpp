#include <StateMachine.hpp>
#include <memory>
#include <chrono>

void main()
{
    //Implement the main loop with delta time
    std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point current_time;
    float delta_time = 0.0f;

    StateMachine state_machine;

    while (!state_machine.is_ending())
    {
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();       
    }
}