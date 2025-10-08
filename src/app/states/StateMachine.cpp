#include <StateMachine.hpp>
#include <iostream>

StateMachine::StateMachine()
{
    this->new_state = nullptr;
}

void StateMachine::add_state(std::unique_ptr<GameState> newState, bool is_replacing)
{
    is_Adding = true;
    this->is_Replacing = is_replacing;
    this->new_state = std::move(newState);
    this->new_state->setStateMachine(this);
}

void StateMachine::remove_state(bool value)
{
    this->is_removing = true;
    this->is_ending = value;
}

void StateMachine::handle_state_changes(float& deltaTime)
{
    if (this->is_removing && !this->states_machine.empty())
    {
        this->states_machine.pop();
        this->is_removing = false;

        if (!this->is_Adding)
        {
            this->states_machine.top()->resume();
            deltaTime = 0.0f;
        }
    }

    if (this->is_Adding)
    {
        if(!this->states_machine.empty())
        {
            if (this->is_Replacing)
            {
                this->states_machine.pop();
            }
        }

        this->states_machine.push(std::move(this->new_state));
        this->states_machine.top()->init();
        this->is_Adding = false;
        deltaTime = 0.0f;
    }
}

