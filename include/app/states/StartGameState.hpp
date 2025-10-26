#pragma once
#include "GameState.hpp"

extern "C" {
    #include <raylib.h>
}
class StartGameState : public GameState {

    protected:
        int selectedOption;
    public:
        StartGameState();
        ~StartGameState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;
};