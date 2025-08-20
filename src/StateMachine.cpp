#include <StateMachine.hpp>

void StateMachine::add_state(std::unique_ptr<GameState> newState, bool is_replacing)
{
    is_Adding = true;
    is_replacing = is_replacing;
    new_state = std::move(new_state);
}

void StateMachine::remove_state(bool value)
{
    is_removing = true;
    is_ending = value;
}

