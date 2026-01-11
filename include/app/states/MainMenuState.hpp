#pragma once
#include "GameState.hpp"

extern "C" {
    #include <raylib.h>
}
class MainMenuState : public GameState {

    protected:
        static const char *MENU_OPTIONS[3];
        static constexpr int OPTION_COUNT = 3;
        int selectedOption = 0;
        const Color SELECTED_BOX_COLOR = {150, 30, 30, 255};
        const Color UNSELECTED_BOX_COLOR = {60, 60, 60, 255};
        const Color SELECTED_OUTLINE_COLOR = RED;
        const Color UNSELECTED_OUTLINE_COLOR = GRAY;
        const Color SELECTED_TEXT_COLOR = YELLOW;
        const Color UNSELECTED_TEXT_COLOR = RAYWHITE;

    public:
        MainMenuState();
        ~MainMenuState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;

        static const int BOX_WIDTH = 350;
        static const int BOX_HEIGHT = 60;
        static const int BOX_SPACING = 40;
};