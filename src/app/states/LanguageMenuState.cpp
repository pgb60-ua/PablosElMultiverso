#include "LanguageMenuState.hpp"
#include "MainMenuState.hpp"
#include "StateMachine.hpp"
#include "I18N.hpp"
#include <string>
#include <cstdlib>
#include <clocale>

extern "C" {
#include <raylib.h>
#include <libintl.h>
}

const char *LanguageMenuState::LANG_OPTIONS[3] = { "English", "Español", "Français" };

static void cambiarIdioma(const std::string& idioma) {
    if (idioma == "en") {
        setenv("LANGUAGE", "en_US", 1);
        setenv("LANG", "en_US.UTF-8", 1);
    } else if (idioma == "es") {
        setenv("LANGUAGE", "es_ES", 1);
        setenv("LANG", "es_ES.UTF-8", 1);
    } else if (idioma == "fr") {
        setenv("LANGUAGE", "fr_FR", 1);
        setenv("LANG", "fr_FR.UTF-8", 1);
    }

    std::string locale_str = idioma + "_" + (idioma == "en" ? "US" : (idioma == "fr" ? "FR" : "ES")) + ".UTF-8";
    setlocale(LC_ALL, locale_str.c_str());

    bindtextdomain("pablos", "./locale");
    textdomain("pablos");
    bind_textdomain_codeset("pablos", "UTF-8");
    
    extern int _nl_msg_cat_cntr;
    ++_nl_msg_cat_cntr;
}

static Rectangle getButtonRect(int index, int screenWidth, int screenHeight) {
    int startY = screenHeight / 2;
    int boxX = (screenWidth - LanguageMenuState::BOX_WIDTH) / 2;
    int boxY = startY + index * (LanguageMenuState::BOX_HEIGHT + LanguageMenuState::BOX_SPACING);
    return Rectangle{(float)boxX, (float)boxY, (float)LanguageMenuState::BOX_WIDTH, (float)LanguageMenuState::BOX_HEIGHT};
}

LanguageMenuState::LanguageMenuState() {}
LanguageMenuState::~LanguageMenuState() {}

void LanguageMenuState::init() {}

void LanguageMenuState::handleInput() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedOption = (selectedOption + 1) % LanguageMenuState::OPTION_COUNT;
    }
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedOption = (selectedOption - 1 + LanguageMenuState::OPTION_COUNT) % LanguageMenuState::OPTION_COUNT;
    }

    bool confirmed = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    
    // Permitir volver atrás con Q o ESC
    if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) {
        state_machine->remove_state(false);
        return;
    }

    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int i = 0; i < LanguageMenuState::OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        if (CheckCollisionPointRec(mousePosition, btn)) {
            selectedOption = i;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                confirmed = true;
            }
        }
    }

    if (confirmed) {
        if (selectedOption == 0) cambiarIdioma("en");
        else if (selectedOption == 1) cambiarIdioma("es");
        else if (selectedOption == 2) cambiarIdioma("fr");
        
        // Volver al menú principal tras seleccionar
        state_machine->remove_state(false); 
    }
}

void LanguageMenuState::update(float deltaTime) {}

void LanguageMenuState::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    const char *title = _("SELECT LANGUAGE");
    int titleFontSize = 40;
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, 1);
    Vector2 titlePos = {(screenWidth - titleSize.x) / 2.0f, (float)screenHeight / 4.0f - titleSize.y / 2.0f};
    DrawTextEx(GetFontDefault(), title, titlePos, (float)titleFontSize, 1.0f, RAYWHITE);

    for (int i = 0; i < LanguageMenuState::OPTION_COUNT; i++) {
        Rectangle btn = getButtonRect(i, screenWidth, screenHeight);
        Color boxColor = (selectedOption == i) ? SELECTED_BOX_COLOR : UNSELECTED_BOX_COLOR;
        Color outlineColor = (selectedOption == i) ? SELECTED_OUTLINE_COLOR : UNSELECTED_OUTLINE_COLOR;
        Color textColor = (selectedOption == i) ? SELECTED_TEXT_COLOR : UNSELECTED_TEXT_COLOR;

        DrawRectangleRec(btn, boxColor);
        DrawRectangleLinesEx(btn, 2, outlineColor);
        
        const char* text = LanguageMenuState::LANG_OPTIONS[i];
        Vector2 textSize = MeasureTextEx(GetFontDefault(), text, 30, 1);

        float textX = btn.x + (btn.width - textSize.x) / 2.0f;
        float textY = btn.y + (btn.height - textSize.y) / 2.0f;

        DrawTextEx(GetFontDefault(), text, {textX, textY}, 30, 1, textColor);
    }
    
    EndDrawing();
}
