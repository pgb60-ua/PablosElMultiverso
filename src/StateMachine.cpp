#include <StateMachine.hpp>

void StateMachine::add_state(std::unique_ptr<GameState> newState, bool is_replacing)
{
    is_Adding = true;
    is_replacing = is_replacing;
    newState->init();
    new_state = std::move(new_state);
}

void StateMachine::remove_state(bool value)
{
    is_removing = true;
    is_ending = value;
}

void StatesMachine::handle_state_changes(float& deltaTime)
{
    if (this->is_removing && !this->states.empty())
    {
        this->states.pop();
        this->is_removing = false;

        if (!this->is_Adding)
        {
            this->states.top()->resume();
            deltaTime = 0.0f;
        }
    }

    if (this->is_Adding)
    {
        if(!this->states_machine.empty())
        {
            if (this->is_replacing)
            {
                this->states.pop();
            }

            this->states.push(std::move(this->new_state));
            this->states.top()->init();
            this->is_Adding = false;
            deltaTime = 0.0f;
        }
    }
}

