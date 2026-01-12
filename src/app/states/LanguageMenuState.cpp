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
    // 1. Configurar la variable de entorno LANGUAGE.
    // Esto es lo que GNU gettext mira para decidir qué archivo .mo cargar.
    // Tiene prioridad sobre el locale configurado (LC_MESSAGES), SIEMPRE que el locale no sea "C".
    std::string langCode = language;
    std::string countryCode = (language == "en" ? "US" : (language == "fr" ? "FR" : "ES"));
    std::string fullCode = langCode + "_" + countryCode; // ej: es_ES
    
    // Prioridad: idioma_PAIS -> idioma -> fallback inglés
    std::string langPriority = fullCode + ":" + langCode + ":en_US:en";
    SetEnvironmentVariable("LANGUAGE", langPriority);

    // 2. Intentar establecer un locale UTF-8 válido.
    // Es CRÍTICO salir del locale "C", porque en "C", gettext ignora la variable LANGUAGE.
    
    // Opción A: Intentar el idioma nativo (ej: fr_FR.UTF-8)
    std::string targetLocale = fullCode + ".UTF-8";
    char* res = setlocale(LC_ALL, targetLocale.c_str());
    
    if (res == nullptr) {
        // Opción B: Si el nativo falta, usar C.UTF-8 (existente en la mayoría de Linux modernos)
        // Esto permite manejar caracteres especiales y activa gettext.
        if (setlocale(LC_ALL, "C.UTF-8") == nullptr) {
            // Opción C: Usar en_US.UTF-8 (muy común)
            if (setlocale(LC_ALL, "en_US.UTF-8") == nullptr) {
                // Opción D: Lo que tenga el sistema por defecto
                setlocale(LC_ALL, "");
            }
        }
    }

    // 3. Recargar dominios
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
