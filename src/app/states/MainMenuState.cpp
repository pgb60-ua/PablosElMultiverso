#include "MainMenuState.hpp"
#include <string>
#include <MainGameState.hpp>
#include <StateMachine.hpp>
extern "C" {
    #include <raylib.h>
}

static const char* MENU_OPTIONS[] = { "Jugar", "Salir" };
static const int MENU_OPTIONS_COUNT = sizeof(MENU_OPTIONS) / sizeof(MENU_OPTIONS[0]);

static Rectangle getButtonRect(int index, int screenWidth, int screenHeight) {
    int startY = screenHeight / 2;
    int boxX = (screenWidth - MainMenuState::BOX_WIDTH) / 2;
    int boxY = startY + index * (MainMenuState::BOX_HEIGHT + MainMenuState::BOX_SPACING);
    return (Rectangle){ (float)boxX, (float)boxY, (float)MainMenuState::BOX_WIDTH, (float)MainMenuState::BOX_HEIGHT };
}

MainMenuState::MainMenuState() {}
MainMenuState::~MainMenuState() {}

void MainMenuState::init() {}
void MainMenuState::handleInput() {

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedOption = (selectedOption + 1) % OPTION_COUNT;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption = (selectedOption - 1 + OPTION_COUNT) % OPTION_COUNT;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
        if (selectedOption == 0) {
            state_machine->add_state(std::make_unique<MainGameState>(), true);
        } else {
            state_machine->remove_state(true);
        }
    }
    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for(int i = 0; i < OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        if (CheckCollisionPointRec(mousePosition, btn)) {
            selectedOption = i;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (selectedOption == 0) {
                    state_machine->add_state(std::make_unique<MainGameState>(), true);
                } else {
                    state_machine->remove_state(true);
                }
            }
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

    for (int i = 0; i < OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        Color boxColor = (selectedOption == i) ? SELECTED_BOX_COLOR : UNSELECTED_BOX_COLOR;
        Color outlineColor = (selectedOption == i) ? SELECTED_OUTLINE_COLOR : UNSELECTED_OUTLINE_COLOR;
        Color textColor = (selectedOption == i) ? SELECTED_TEXT_COLOR : UNSELECTED_TEXT_COLOR;

        DrawRectangleRec(btn, boxColor);
        DrawRectangleLinesEx(btn, 2, outlineColor);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), MENU_OPTIONS[i], 30, 1);
        
        DrawText(MENU_OPTIONS[i], 
                (int)btn.x + (btn.width - textSize.x)/2, 
                (int)btn.y + (btn.height - textSize.y)/2, 
                30, 
                textColor);
    }
    
    EndDrawing();

}
void MainMenuState::pause() {}
void MainMenuState::resume() {}