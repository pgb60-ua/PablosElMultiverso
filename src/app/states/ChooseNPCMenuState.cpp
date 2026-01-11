#include "ChooseNPCMenuState.hpp"
#include "DataFileManager.hpp"
#include "MainGameState.hpp"
#include "MainMenuState.hpp"
#include "SpriteLoaderManager.hpp"
#include "StateMachine.hpp"
#include <string>
extern "C"
{
#include <raylib.h>
}

ChooseNPCMenuState::ChooseNPCMenuState() {}

ChooseNPCMenuState::~ChooseNPCMenuState() {}

std::string ChooseNPCMenuState::GetDefaultCharacterName(PLAYER_TYPE type) const
{
    switch (type)
    {
    case PLAYER_TYPE::MAGE:
        return "Mage";
    case PLAYER_TYPE::RANGE:
        return "Ranger";
    case PLAYER_TYPE::WARRIOR:
        return "Warrior";
    default:
        return "Character";
    }
}

void ChooseNPCMenuState::LoadCharacterData(CharacterOption &character, PLAYER_TYPE type)
{
    DataFileManager &dataManager = DataFileManager::GetInstance();

    try
    {
        // Obtener datos del personaje
        const DataMap &characterData = dataManager.GetData(type);

        // Obtener nombre
        if (characterData.count("name") && std::holds_alternative<std::string>(characterData.at("name")))
            character.name = std::get<std::string>(characterData.at("name"));
        else
            character.name = GetDefaultCharacterName(type);

        // Obtener descripción
        if (characterData.count("description") && std::holds_alternative<std::string>(characterData.at("description")))
            character.description = std::get<std::string>(characterData.at("description"));
        else
            character.description = "";
    }
    catch (const std::exception &e)
    {
        character.name = GetDefaultCharacterName(type);
        character.description = "";
    }
}

void ChooseNPCMenuState::LoadCharacterSprites()
{
    std::vector<PLAYER_TYPE> types = {PLAYER_TYPE::MAGE, PLAYER_TYPE::RANGE, PLAYER_TYPE::WARRIOR};

    for (PLAYER_TYPE type : types)
    {
        CharacterOption character;
        character.type = type;
        LoadCharacterData(character, type);
        characters.push_back(character);
    }
}


void ChooseNPCMenuState::init()
{
    // Cargar los personajes con sus datos y sprites
    LoadCharacterSprites();

    currentCharacterIndex = 0;
    arrowHovered[0] = false;
    arrowHovered[1] = false;
}

void ChooseNPCMenuState::handleInput()
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

    // Detectar hover del ratón sobre los botones de flecha
    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (!characters.empty() && currentCharacterIndex < static_cast<int>(characters.size()))
    {

        // Calcular posición del contenedor
        Vector2 containerPos = GetContainerPos(screenWidth, screenHeight);

        // Resetear el array de hover
        arrowHovered[0] = false;
        arrowHovered[1] = false;

        // Verificar hover sobre cada botón
        for (int i = 0; i < 2; i++)
        {
            Rectangle btn = GetArrowButtonRect(i, containerPos, screenHeight);
            if (CheckCollisionPointRec(mousePosition, btn))
            {
                arrowHovered[i] = true;

                // Si se hace clic, navegar
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (i == 0) // Flecha izquierda
                    {
                        currentCharacterIndex = (currentCharacterIndex - 1 + characters.size()) % characters.size();
                    }
                    else // Flecha derecha
                    {
                        currentCharacterIndex = (currentCharacterIndex + 1) % characters.size();
                    }
                }
            }
        }

        // Verificar clic en el recuadro del personaje para seleccionarlo
        Rectangle containerRect = {containerPos.x, containerPos.y, DISPLAY_SIZE, DISPLAY_SIZE};

        if (CheckCollisionPointRec(mousePosition, containerRect))
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                // Confirmar selección del personaje actual
                state_machine->add_state(std::make_unique<MainGameState>(characters[currentCharacterIndex].type), true);
            }
        }
    }
}

void ChooseNPCMenuState::update(float deltaTime)
{
    if (!characters.empty() && currentCharacterIndex < static_cast<int>(characters.size()))
    {
        CharacterOption &character = characters[currentCharacterIndex];
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(character.type);

        character.spriteAnimation.timeAccumulator += deltaTime;
        if (character.spriteAnimation.timeAccumulator >= character.spriteAnimation.FRAME_DURATION)
        {
            character.spriteAnimation.timeAccumulator -= character.spriteAnimation.FRAME_DURATION;
            character.spriteAnimation.frameIndex++;
            character.spriteAnimation.frameIndex %= sheet.spriteFrameCount;
        }
    }
}

void ChooseNPCMenuState::DrawCenteredText(const char *text, float y, int fontSize, Color color) const
{
    int screenWidth = GetScreenWidth();
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 textPos = {(screenWidth - textSize.x) / 2.0f, y};
    DrawTextEx(GetFontDefault(), text, textPos, (float)fontSize, 1.0f, color);
}

Vector2 ChooseNPCMenuState::GetContainerPos(int screenWidth, int screenHeight) const
{
    return Vector2{(screenWidth - DISPLAY_SIZE) / 2.0f, (screenHeight - DISPLAY_SIZE) / 2.0f - 20.0f};
}

Vector2 ChooseNPCMenuState::GetSpritePos(const Vector2 &containerPos, float spriteWidth, float spriteHeight) const
{
    return Vector2{containerPos.x + (DISPLAY_SIZE - spriteWidth) / 2.0f,
                   containerPos.y + (DISPLAY_SIZE - spriteHeight) / 2.0f};
}

Rectangle ChooseNPCMenuState::GetArrowButtonRect(int index, const Vector2 &containerPos, int screenHeight)
{
    const char *arrow = (index == 0) ? "<" : ">";
    Vector2 arrowSize = MeasureTextEx(GetFontDefault(), arrow, ARROW_FONT_SIZE, 1);

    float xPos = (index == 0) ? containerPos.x - ARROW_DISTANCE
                              : containerPos.x + DISPLAY_SIZE + ARROW_DISTANCE - arrowSize.x;
    float yPos = (screenHeight - arrowSize.y) / 2.0f;

    return Rectangle{xPos - ARROW_PADDING, yPos - ARROW_PADDING,
                     arrowSize.x + ARROW_PADDING * 2, arrowSize.y + ARROW_PADDING * 2};
}

void ChooseNPCMenuState::DrawNavigationArrows(const Vector2 &containerPos) const
{
    int screenHeight = GetScreenHeight();
    Color fillColor = {60, 60, 60, 255};
    const char *arrows[] = {"<", ">"};

    for (int i = 0; i < 2; i++)
    {
        Rectangle rect = GetArrowButtonRect(i, containerPos, screenHeight);
        Vector2 arrowSize = MeasureTextEx(GetFontDefault(), arrows[i], ARROW_FONT_SIZE, 1);

        // Calcular posición del texto
        float xPos = (i == 0) ? containerPos.x - ARROW_DISTANCE
                              : containerPos.x + DISPLAY_SIZE + ARROW_DISTANCE - arrowSize.x;
        Vector2 arrowPos = {xPos, (screenHeight - arrowSize.y) / 2.0f};

        // Color según hover
        Color arrowColor = (arrowHovered[i] && characters.size() > 1) ? SELECTED_ARROW_COLOR : UNSELECTED_ARROW_COLOR;

        DrawRectangleRec(rect, fillColor);
        DrawRectangleLinesEx(rect, 1.0f, arrowColor);
        DrawTextEx(GetFontDefault(), arrows[i], arrowPos, (float)ARROW_FONT_SIZE, 1.0f, arrowColor);
    }
}
void ChooseNPCMenuState::render()
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
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(currentChar.type);
        Rectangle frame = sheet.frames[currentChar.spriteAnimation.frameIndex];

        // Posición del cuadrado contenedor (centrado)
        Vector2 containerPos = GetContainerPos(screenWidth, screenHeight);

        // Dibujar fondo del sprite
        Color bgColor = {40, 40, 40, 255};
        DrawRectangle((int)containerPos.x, (int)containerPos.y, (int)DISPLAY_SIZE, (int)DISPLAY_SIZE, bgColor);
        DrawRectangleLines((int)containerPos.x - 1, (int)containerPos.y - 1, (int)DISPLAY_SIZE + 2,
                           (int)DISPLAY_SIZE + 2, YELLOW);

        // Calcular tamaño del sprite
        float spriteWidth = frame.width * CONTAINER_SCALE;
        float spriteHeight = frame.height * CONTAINER_SCALE;

        // Calcular posición del sprite para centrarlo dentro del contenedor
        Vector2 spritePos = GetSpritePos(containerPos, spriteWidth, spriteHeight);

        // Dibujar sprite
        if (sheet.texture.id > 0)
        {
            Rectangle destRec = {spritePos.x, spritePos.y, spriteWidth, spriteHeight};
            DrawTexturePro(sheet.texture, frame, destRec, {0, 0}, 0.0f, currentChar.spriteAnimation.color);
        }

        // Dibujar nombre del personaje
        DrawCenteredText(currentChar.name.c_str(), containerPos.y + DISPLAY_SIZE + 30.0f, 35, YELLOW);

        // Dibujar descripción si existe
        if (!currentChar.description.empty())
        {
            float descY = containerPos.y + DISPLAY_SIZE + 70.0f; // 30 + 40
            DrawCenteredText(currentChar.description.c_str(), descY, 18, GRAY);
        }

        // Dibujar flechas de navegación
        DrawNavigationArrows(containerPos);

        // Indicador de personaje actual
        const char *indicator = TextFormat("%d / %d", currentCharacterIndex + 1, (int)characters.size());
        DrawCenteredText(indicator, screenHeight - 100.0f, 20, GRAY);
    }

    // Instrucciones
    DrawCenteredText("Flechas: Cambiar | ENTER: Confirmar | Q: Volver | ESC: Salir del Juego", screenHeight - 50.0f, 20,
                     DARKGRAY);

    DrawFPS(GetScreenWidth() - 100, 10);
    EndDrawing();
}

void ChooseNPCMenuState::pause() {}
void ChooseNPCMenuState::resume() {}
