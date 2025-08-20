#pragma once
#include <string>
#include <memory>

class StateMachine;

class GameState
{

    public:
        GameState();
        virtual ~GameState() = default;

        virtual void handleInput() = 0;
        virtual void update(float deltaTime) = 0;
        virtual void render() = 0;

        const std::string& getName() const { return name; }
        void setStateMachine(StateMachine* stt_mch) {state_machine = stt_mch;}

    protected:
        std::string name;
        StateMachine* state_machine;
};