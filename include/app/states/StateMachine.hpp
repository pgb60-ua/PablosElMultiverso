#pragma once
#include <GameState.hpp>
#include <stack>
#include <memory>
#include <unordered_map>
#include <string>

class StateMachine 
{
    public:
        StateMachine();
        ~StateMachine() = default;

        void add_state(std::unique_ptr<GameState> state, bool is_replacing);
        void remove_state(bool value);
        void handle_state_changes(float& deltaTime);

        void stop() {is_running = false;}
        bool isRunning() {return this->is_running;}

        bool is_game_ending() {return this->is_ending;}

        std::unique_ptr<GameState>& getCurrentState() {return this->states_machine.top();}
    
    private:
        std::stack<std::unique_ptr<GameState>> states_machine;
        std::unique_ptr<GameState> new_state;
        bool is_running;

        bool is_removing = false,
             is_Adding = false,
             is_Replacing = false,
             is_ending = false;
};