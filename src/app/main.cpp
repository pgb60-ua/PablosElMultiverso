#include "ItemsFactory.hpp"
#include "ScreenConstants.hpp"
#include "WindowFlags.hpp"
#include <MainGameState.hpp>
#include <MainMenuState.hpp>
#include <StateMachine.hpp>
#include <memory>
#include "I18N.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
extern "C"
{
#include <raylib.h>
}

int main()
{
    if (setlocale(LC_ALL, "") == nullptr)
    {
        std::cerr << "Warning: Failed to set locale from environment." << std::endl;
    }
    
    // Configuración específica para la localización
    bindtextdomain("pablos", "./locale");
    textdomain("pablos");

    // Crear ventana con el tamaño del monitor
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pablos, El Multiverso");

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    float delta_time = 0.0f;

    StateMachine state_machine = StateMachine();
    state_machine.add_state(std::make_unique<MainMenuState>(), false);
    state_machine.handle_state_changes(delta_time);
    SetTargetFPS(120);
    InitAudioDevice();
    DataFileManager::GetInstance().DetectAndSetAssetsPath();
    SpriteLoaderManager::GetInstance().DetectAndSetAssetsPath();
    ItemsFactory::GetInstance().LoadAllItems();
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
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
