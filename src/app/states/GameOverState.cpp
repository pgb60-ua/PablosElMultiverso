#include "GameOverState.hpp"
#include <string>
#include <MainGameState.hpp>
#include <MainMenuState.hpp>
#include <StateMachine.hpp>
extern "C" {
    #include <raylib.h>
}

static const char* MENU_OPTIONS[] = { "Volver a jugar", "Volver al menú principal" };
static const int OPTION_COUNT = sizeof(MENU_OPTIONS) / sizeof(MENU_OPTIONS[0]);

static Rectangle getButtonRect(int index, int screenWidth, int screenHeight) {
    int startY = screenHeight / 2;
    int boxX = (screenWidth - GameOverState::BOX_WIDTH) / 2;
    int boxY = startY + index * (GameOverState::BOX_HEIGHT + GameOverState::BOX_SPACING);
    return (Rectangle){ (float)boxX, (float)boxY, (float)GameOverState::BOX_WIDTH, (float)GameOverState::BOX_HEIGHT };
}

GameOverState::GameOverState() {}
GameOverState::~GameOverState() {}

void GameOverState::init() {}
void GameOverState::handleInput() {

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
            state_machine->add_state(std::make_unique<MainMenuState>(), true);
        }
    }

    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int i = 0; i < OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        if (CheckCollisionPointRec(mousePosition, btn)) {
            selectedOption = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (selectedOption == 0) {
                    state_machine->add_state(std::make_unique<MainGameState>(), true);
                } else {
                    state_machine->remove_state(true);
                }
            }
        }
    }

}
void GameOverState::update(float deltaTime) {}

void GameOverState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    
    const char* title = "HAS PERDIDO!";
    int titleFontSize = 50;
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, 1);
    Vector2 titlePos = { (screenWidth - titleSize.x) / 2.0f, (float)screenHeight / 4.0f - titleSize.y / 2.0f };
    DrawTextEx(GetFontDefault(), title, titlePos, (float)titleFontSize, 1.0f, RAYWHITE);

    for (int i = 0; i < OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);

        Color boxColor = (selectedOption == i) ? SELECTED_BOX_COLOR : UNSELECTED_BOX_COLOR;
        Color outlineColor = (selectedOption == i) ? SELECTED_OUTLINE_COLOR : UNSELECTED_OUTLINE_COLOR;
        Color textColor = (selectedOption == i) ? SELECTED_TEXT_COLOR : UNSELECTED_TEXT_COLOR;

        DrawRectangleRec(btn, boxColor);
        DrawRectangleLinesEx(btn, 2, outlineColor);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), MENU_OPTIONS[i], 30, 1);
        Vector2 textPos = { btn.x + (btn.width - textSize.x) / 2.0f,
                    btn.y + (btn.height - textSize.y) / 2.0f };
        DrawTextEx(GetFontDefault(), MENU_OPTIONS[i], textPos, 30.0f, 1.0f, textColor);
    }
    
    EndDrawing();

}
void GameOverState::pause() {}
void GameOverState::resume() {}