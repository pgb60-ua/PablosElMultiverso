#pragma once
#include <GameState.hpp>

class MainMenuState : public GameState
{
    public:
        MainMenuState();
        ~MainMenuState() = default;

        void init() override;
        void handleInput() override;
        void update(float deltaTime) override;
        void render() override;

        void pause(){};
        void resume(){};

    
    private:
        char entered_key;
};