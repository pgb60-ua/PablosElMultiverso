#pragma once
#include "GameState.hpp"

extern "C" {
    #include <raylib.h>
}
class MainMenuState : public GameState {

    protected:
        int selectedOption = 0;
        const int optionCount = 2;
        const Color SELECTED_BOX_COLOR = {150, 30, 30, 255};
        const Color UNSELECTED_BOX_COLOR = {60, 60, 60, 255};
    public:
        MainMenuState();
        ~MainMenuState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;
};