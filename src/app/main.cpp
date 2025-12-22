#include "WindowFlags.hpp"
#include <MainGameState.hpp>
#include <MainMenuState.hpp>
#include <StateMachine.hpp>
#include <AudioManager.hpp>
#include <memory>
extern "C"
{
#include <raylib.h>
}

int main()
{
    // Crear ventana con el tamaño del monitor
    InitWindow(1200, 800, "Pablos, El Multiverso");

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainMenuState>(), false);
    state_machine.handle_state_changes(delta_time);
    SetTargetFPS(120);
    InitAudioDevice();
    DataFileManager::GetInstance().DetectAndSetAssetsPath();
    AudioManager::GetInstance().DetectAndSetAssetsPath();
    SpriteLoaderManager::GetInstance().DetectAndSetAssetsPath();

    while (!state_machine.is_game_ending() && !WindowShouldClose())
    {
        HandleWindowFlags();
        delta_time = GetFrameTime();
        state_machine.handle_state_changes(delta_time);
        state_machine.getCurrentState()->handleInput();
        state_machine.getCurrentState()->update(delta_time);
        state_machine.getCurrentState()->render();
    }
    SpriteLoaderManager::GetInstance().ClearCache();
    AudioManager::GetInstance().ClearCache();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
