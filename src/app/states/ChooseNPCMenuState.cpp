#include "ChooseNPCGameState.hpp"
#include "MainGameState.hpp"
#include "MainMenuState.hpp"
#include "StateMachine.hpp"
#include "DataFileManager.hpp"
#include "SpriteLoaderManager.hpp"
#include <string>
extern "C"
{
#include <raylib.h>
}

ChooseNPCGameState::ChooseNPCGameState() {}

ChooseNPCGameState::~ChooseNPCGameState() {}

std::string ChooseNPCGameState::GetDefaultCharacterName(PLAYER_TYPE type) const
{
    switch (type)
    {
    case PLAYER_TYPE::MAGE: return "Mage";
    case PLAYER_TYPE::RANGE: return "Ranger";
    case PLAYER_TYPE::WARRIOR: return "Warrior";
    case PLAYER_TYPE::HEALER: return "Healer";
    default: return "Character";
    }
}

void ChooseNPCGameState::LoadCharacterData(CharacterOption& character, PLAYER_TYPE type)
{
    auto& dataManager = DataFileManager::GetInstance();
    auto& spriteManager = SpriteLoaderManager::GetInstance();

    try
    {
        // Obtener datos del personaje
        const DataMap& characterData = dataManager.GetData(type);

        // Obtener nombre del JSON de datos
        auto nameIt = characterData.find("name");
        if (nameIt != characterData.end() && std::holds_alternative<std::string>(nameIt->second))
        {
            character.name = std::get<std::string>(nameIt->second);
            TraceLog(LOG_INFO, "Loaded character name: %s", character.name.c_str());
        }
        else
        {
            character.name = GetDefaultCharacterName(type);
            TraceLog(LOG_WARNING, "Using default name for character type");
        }

        // Obtener descripción
        auto descIt = characterData.find("description");
        character.description = (descIt != characterData.end() && std::holds_alternative<std::string>(descIt->second))
                          ? std::get<std::string>(descIt->second)
                          : "";
        
        TraceLog(LOG_INFO, "Character description: %s", character.description.c_str());

        // Obtener sprite sheet
        const SpriteSheet& spriteSheet = spriteManager.GetSpriteSheet(type);
        character.spriteSheet = spriteSheet.texture;
        character.spriteSheetPath = "";
        
        TraceLog(LOG_INFO, "Loaded sprite sheet with texture ID: %d, frames: %zu", 
                 spriteSheet.texture.id, spriteSheet.frames.size());

        // Convertir Rectangle a SpriteFrame
        for (const auto& rect : spriteSheet.frames)
        {
            SpriteFrame frame;
            frame.x = (int)rect.x;
            frame.y = (int)rect.y;
            frame.width = (int)rect.width;
            frame.height = (int)rect.height;
            character.frames.push_back(frame);
        }

        character.currentFrame = 0;
        TraceLog(LOG_INFO, "Successfully loaded character: %s with %zu frames", 
                 character.name.c_str(), character.frames.size());
    }
    catch (const std::exception& e)
    {
        // Si falla, crear un personaje por defecto
        TraceLog(LOG_ERROR, "Failed to load character data: %s", e.what());
        character.name = GetDefaultCharacterName(type);
        character.description = "";
        character.currentFrame = 0;
        TraceLog(LOG_WARNING, "Using fallback character: %s", character.name.c_str());
    }
}

void ChooseNPCGameState::LoadCharacterSprites()
{
    std::vector<PLAYER_TYPE> types = {
        PLAYER_TYPE::MAGE,
        PLAYER_TYPE::RANGE,
        PLAYER_TYPE::WARRIOR
    };

    for (PLAYER_TYPE type : types)
    {
        CharacterOption character;
        character.type = type;
        LoadCharacterData(character, type);
        characters.push_back(character);
    }
}

SpriteFrame ChooseNPCGameState::GetCurrentFrame(const CharacterOption& character) const
{
    if (!character.frames.empty() && character.currentFrame < static_cast<int>(character.frames.size()))
    {
        return character.frames[character.currentFrame];
    }

    // Frame por defecto si no hay datos
    return SpriteFrame{0, 0, 64, 64};
}

void ChooseNPCGameState::init()
{
    // Detectar y establecer la ruta de assets
    DataFileManager::GetInstance().DetectAndSetAssetsPath();
    SpriteLoaderManager::GetInstance().DetectAndSetAssetsPath();

    // Cargar los personajes con sus datos y sprites
    LoadCharacterSprites();

    currentCharacterIndex = 0;
}

void ChooseNPCGameState::handleInput()
{
    // Navegación con flechas horizontales
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        currentCharacterIndex = (currentCharacterIndex + 1) % characters.size();
    }

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        currentCharacterIndex = (currentCharacterIndex - 1 + characters.size()) % characters.size();
    }

    // Confirmar selección
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
    {
        state_machine->add_state(std::make_unique<MainGameState>(characters[currentCharacterIndex].type), true);
    }

    // Volver al menú anterior (reemplazar estado actual con MainMenu)
    if (IsKeyPressed(KEY_Q))
    {
        state_machine->add_state(std::make_unique<MainMenuState>(), true);
    }
}

void ChooseNPCGameState::update(float deltaTime __attribute__((unused)))
{
    // Por ahora no hay lógica de actualización
    // Aquí se podría agregar animación de sprites si se desea
}

void ChooseNPCGameState::DrawCenteredText(const char* text, float y, int fontSize, Color color) const
{
    int screenWidth = GetScreenWidth();
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 textPos = {(screenWidth - textSize.x) / 2.0f, y};
    DrawTextEx(GetFontDefault(), text, textPos, (float)fontSize, 1.0f, color);
}

void ChooseNPCGameState::DrawNavigationArrows(const Vector2& spritePos, float spriteWidth) const
{
    int screenHeight = GetScreenHeight();
    int arrowFontSize = 60;
    Color arrowColor = (characters.size() > 1) ? SELECTED_ARROW_COLOR : UNSELECTED_ARROW_COLOR;

    // Flecha izquierda
    const char* leftArrow = "<";
    Vector2 leftArrowSize = MeasureTextEx(GetFontDefault(), leftArrow, arrowFontSize, 1);
    Vector2 leftArrowPos = {
        spritePos.x - 120.0f,
        (screenHeight - leftArrowSize.y) / 2.0f
    };
    DrawTextEx(GetFontDefault(), leftArrow, leftArrowPos, (float)arrowFontSize, 1.0f, arrowColor);

    // Flecha derecha
    const char* rightArrow = ">";
    Vector2 rightArrowSize = MeasureTextEx(GetFontDefault(), rightArrow, arrowFontSize, 1);
    Vector2 rightArrowPos = {
        spritePos.x + spriteWidth + 120.0f - rightArrowSize.x,
        (screenHeight - rightArrowSize.y) / 2.0f
    };
    DrawTextEx(GetFontDefault(), rightArrow, rightArrowPos, (float)arrowFontSize, 1.0f, arrowColor);
}

void ChooseNPCGameState::render()
{
    BeginDrawing();
    ClearBackground(BLACK);

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Dibujar título
    DrawCenteredText("SELECCIONA TU PERSONAJE", 50.0f, 40, RAYWHITE);

    if (!characters.empty() && currentCharacterIndex < static_cast<int>(characters.size()))
    {
        CharacterOption &currentChar = characters[currentCharacterIndex];
        SpriteFrame frame = GetCurrentFrame(currentChar);

        // Tamaño fijo del cuadrado contenedor
        const float containerSize = 64.0f;
        const float containerScale = 6.0f; // Escala para visualización
        const float displaySize = containerSize * containerScale;

        // Posición del cuadrado contenedor (centrado)
        Vector2 containerPos = {
            (screenWidth - displaySize) / 2.0f,
            (screenHeight - displaySize) / 2.0f - 20.0f
        };

        // Dibujar fondo del sprite (cuadrado fijo de 64x64 escalado)
        DrawRectangle(
            (int)containerPos.x,
            (int)containerPos.y,
            (int)displaySize,
            (int)displaySize,
            (Color){40, 40, 40, 255}
        );
        // Línea amarilla 
        DrawRectangleLines(
            (int)containerPos.x - 1,
            (int)containerPos.y - 1,
            (int)displaySize + 2,
            (int)displaySize + 2,
            YELLOW
        );

        // Calcular tamaño del sprite 
        float spriteWidth = frame.width * containerScale;
        float spriteHeight = frame.height * containerScale;

        // Calcular posición del sprite para centrarlo dentro del contenedor
        Vector2 spritePos = {
            containerPos.x + (displaySize - spriteWidth) / 2.0f,
            containerPos.y + (displaySize - spriteHeight) / 2.0f
        };

        // Dibujar sprite
        if (currentChar.spriteSheet.id > 0) {
            Rectangle sourceRec = {(float)frame.x, (float)frame.y, (float)frame.width, (float)frame.height};
            Rectangle destRec = {spritePos.x, spritePos.y, spriteWidth, spriteHeight};
            DrawTexturePro(currentChar.spriteSheet, sourceRec, destRec, {0, 0}, 0.0f, WHITE);
        }

        // Dibujar nombre del personaje
        DrawCenteredText(currentChar.name.c_str(), containerPos.y + displaySize + 30.0f, 35, YELLOW);

        // Dibujar descripción si existe
        if (!currentChar.description.empty())
        {
            float descY = containerPos.y + displaySize + 70.0f; // 30 + 40
            DrawCenteredText(currentChar.description.c_str(), descY, 18, GRAY);
        }

        // Dibujar flechas de navegación
        DrawNavigationArrows(containerPos, displaySize);

        // Indicador de personaje actual
        const char* indicator = TextFormat("%d / %d", currentCharacterIndex + 1, (int)characters.size());
        DrawCenteredText(indicator, screenHeight - 100.0f, 20, GRAY);
    }

    // Instrucciones
    DrawCenteredText("Flechas: Cambiar | ENTER: Confirmar | Q: Volver | ESC: Salir del Juego", 
                     screenHeight - 50.0f, 20, DARKGRAY);

    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
}

void ChooseNPCGameState::pause() {}
void ChooseNPCGameState::resume() {}
