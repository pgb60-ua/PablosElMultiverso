#include "LanguageMenuState.hpp"
#include "MainMenuState.hpp"
#include "StateMachine.hpp"
#include "I18N.hpp"
#include <string>
#include <cstdlib>
#include <clocale>
#include <libintl.h>
extern "C" {
#include <raylib.h>
}

const char *LanguageMenuState::LANG_OPTIONS[3] = { "English", "Español", "Français" };

static void changeLanguage(const std::string& language) {
    // Configurar EXCLUSIVAMENTE el idioma deseado en LANGUAGE. 
    // Sin fallbacks a inglés en la variable, para forzar el uso del .mo específico.
    std::string langEnv;
    if (language == "fr") langEnv = "fr_FR:fr";
    else if (language == "es") langEnv = "es_ES:es";
    else langEnv = "en_US:en";
    
    SetEnvironmentVariable("LANGUAGE", langEnv);

    // Intentar activar el locale nativo (ej: fr_FR.UTF-8).
    std::string target = (language == "fr" ? "fr_FR.UTF-8" : (language == "es" ? "es_ES.UTF-8" : "en_US.UTF-8"));
    
    // Si el locale nativo no está instalado en el sistema, gettext ignorará LANGUAGE
    // a menos que salgamos del modo "C" (ASCII).
    // Usamos en_US.UTF-8 como locale "puente" porque suele estar instalado siempre.
    // Esto permite que el sistema funcione en modo UTF-8 (necesario) y gettext
    // inyecte las traducciones de LANGUAGE (francés) sobre ese entorno técnico.
    if (setlocale(LC_ALL, target.c_str()) == nullptr) {
        if (setlocale(LC_ALL, "en_US.UTF-8") == nullptr) {
             setlocale(LC_ALL, ""); 
        }
    }

    bindtextdomain("pablos", GetLocalePath().c_str());
    bind_textdomain_codeset("pablos", "UTF-8");
    textdomain("pablos");
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
        if (selectedOption == 0) changeLanguage("en");
        else if (selectedOption == 1) changeLanguage("es");
        else if (selectedOption == 2) changeLanguage("fr");
        
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
