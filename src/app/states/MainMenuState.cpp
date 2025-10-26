#include "MainMenuState.hpp"
#include <string>
#include <MainGameState.hpp>
#include <StateMachine.hpp>
extern "C" {
    #include <raylib.h>
}


MainMenuState::MainMenuState() {}
MainMenuState::~MainMenuState() {}

void MainMenuState::init() {}
void MainMenuState::handleInput() {

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedOption = (selectedOption + 1) % optionCount;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption = (selectedOption - 1 + optionCount) % optionCount;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        if (selectedOption == 0) {
            state_machine->add_state(std::make_unique<MainGameState>(), true);
        } else {
            state_machine->remove_state(true);
        }
    }

}
void MainMenuState::update(float deltaTime) {}

void MainMenuState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    const char* title = "PABLOS, EL MULTIVERSO";
    int titleFontSize = 50;
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, 1);
    DrawText(title, (screenWidth - titleSize.x)/2, screenHeight/4, titleFontSize, RAYWHITE);
    
    int boxWidth = 200;
    int boxHeight = 60;
    int boxSpacing = 40;
    int startY = screenHeight/2;
    
    const char* options[] = { "Jugar", "Salir" };
    for (int i = 0; i < optionCount; i++) {
        Vector2 textSize = MeasureTextEx(GetFontDefault(), options[i], 30, 1);
        int boxX = (screenWidth - boxWidth)/2;
        int boxY = startY + i * (boxHeight + boxSpacing);
        
        Color boxColor = (selectedOption == i) ? SELECTED_BOX_COLOR : UNSELECTED_BOX_COLOR;        
        DrawRectangle(boxX, boxY, boxWidth, boxHeight, boxColor);
        DrawRectangleLinesEx((Rectangle){(float)boxX, (float)boxY, (float)boxWidth, (float)boxHeight}, 2, 
                            (selectedOption == i) ? RED : GRAY);
        
        DrawText(options[i], 
                boxX + (boxWidth - textSize.x)/2, 
                boxY + (boxHeight - textSize.y)/2, 
                30, 
                (selectedOption == i) ? YELLOW : RAYWHITE);
    }
    
    EndDrawing();

}
void MainMenuState::pause() {}
void MainMenuState::resume() {}