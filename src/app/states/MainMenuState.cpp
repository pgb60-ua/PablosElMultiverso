#include "MainMenuState.hpp"
#include "ChooseNPCMenuState.hpp"
#include "LanguageMenuState.hpp"
#include "StateMachine.hpp"
#include <string>
#include "I18N.hpp"
extern "C"
{
#include <raylib.h>
}

const char *MainMenuState::MENU_OPTIONS[3] = { N_("Play"), N_("Change Language"), N_("Exit") };


static Rectangle getButtonRect(int index, int screenWidth, int screenHeight)
{
    int startY = screenHeight / 2;
    int boxX = (screenWidth - MainMenuState::BOX_WIDTH) / 2;
    int boxY = startY + index * (MainMenuState::BOX_HEIGHT + MainMenuState::BOX_SPACING);
    return Rectangle{(float)boxX, (float)boxY, (float)MainMenuState::BOX_WIDTH, (float)MainMenuState::BOX_HEIGHT};
}

MainMenuState::MainMenuState() {}
MainMenuState::~MainMenuState() {}

void MainMenuState::init() {}
void MainMenuState::handleInput()
{

    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedOption = (selectedOption + 1) % MainMenuState::OPTION_COUNT;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedOption = (selectedOption - 1 + MainMenuState::OPTION_COUNT) % MainMenuState::OPTION_COUNT;
    }

    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
    {
        if (selectedOption == 0)
        {
            state_machine->add_state(std::make_unique<ChooseNPCMenuState>(), true);
        }
        else if (selectedOption == 1)
        {
            state_machine->add_state(std::make_unique<LanguageMenuState>(), false);
        }
        else
        {
            state_machine->remove_state(true);
        }
    }
    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int i = 0; i < MainMenuState::OPTION_COUNT; i++)
    {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        if (CheckCollisionPointRec(mousePosition, btn))
        {
            selectedOption = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (selectedOption == 0)
                {
                    state_machine->add_state(std::make_unique<ChooseNPCMenuState>(), true);
                }
                else if (selectedOption == 1)
                {
                    state_machine->add_state(std::make_unique<LanguageMenuState>(), false);
                }
                else
                {
                    state_machine->remove_state(true);
                }
            }
        }
    }
}
void MainMenuState::update(float deltaTime) {}

void MainMenuState::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *title = _("PABLOS, THE MULTIVERSE");
    int titleFontSize = 50;
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, 1);
    Vector2 titlePos = {(screenWidth - titleSize.x) / 2.0f, (float)screenHeight / 4.0f - titleSize.y / 2.0f};
    DrawTextEx(GetFontDefault(), title, titlePos, (float)titleFontSize, 1.0f, RAYWHITE);

    for (int i = 0; i < MainMenuState::OPTION_COUNT; i++)
    {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        Color boxColor = (selectedOption == i) ? SELECTED_BOX_COLOR : UNSELECTED_BOX_COLOR;
        Color outlineColor = (selectedOption == i) ? SELECTED_OUTLINE_COLOR : UNSELECTED_OUTLINE_COLOR;
        Color textColor = (selectedOption == i) ? SELECTED_TEXT_COLOR : UNSELECTED_TEXT_COLOR;

        DrawRectangleRec(btn, boxColor);
        DrawRectangleLinesEx(btn, 2, outlineColor);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), _(MainMenuState::MENU_OPTIONS[i]), 30, 1);

        float textX = btn.x + (btn.width - textSize.x) / 2.0f;
        float textY = btn.y + (btn.height - textSize.y) / 2.0f;

        DrawTextEx(GetFontDefault(), _(MainMenuState::MENU_OPTIONS[i]), {textX, textY}, 30, 1, textColor);
    }
    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
}
void MainMenuState::pause() {}
void MainMenuState::resume() {}
