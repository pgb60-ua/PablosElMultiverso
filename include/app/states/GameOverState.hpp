#pragma once
#include "GameState.hpp"

extern "C" {
    #include <raylib.h>
}
class GameOverState : public GameState {

    protected:
        int selectedOption = 0;
        static const char *MENU_OPTIONS[2];
        static constexpr int OPTION_COUNT = 2;

        const Color SELECTED_BOX_COLOR = {150, 30, 30, 255};
        const Color UNSELECTED_BOX_COLOR = {60, 60, 60, 255};
        const Color SELECTED_OUTLINE_COLOR = RED;
        const Color UNSELECTED_OUTLINE_COLOR = GRAY;
        const Color SELECTED_TEXT_COLOR = YELLOW;
        const Color UNSELECTED_TEXT_COLOR = RAYWHITE;
    public:
        GameOverState();
        ~GameOverState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;

        static const int BOX_WIDTH = 400;
        static const int BOX_HEIGHT = 60;
        static const int BOX_SPACING = 40;
};