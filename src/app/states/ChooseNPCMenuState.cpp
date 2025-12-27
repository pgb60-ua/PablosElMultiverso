#include "ChooseNPCGameState.hpp"
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

ChooseNPCGameState::ChooseNPCGameState() {}

ChooseNPCGameState::~ChooseNPCGameState() {}

std::string ChooseNPCGameState::GetDefaultCharacterName(PLAYER_TYPE type) const
{
    switch (type)
    {
    case PLAYER_TYPE::MAGE:
        return "Mage";
    case PLAYER_TYPE::RANGE:
        return "Ranger";
    case PLAYER_TYPE::WARRIOR:
        return "Warrior";
    case PLAYER_TYPE::HEALER:
        return "Healer";
    default:
        return "Character";
    }
}

void ChooseNPCGameState::LoadCharacterData(CharacterOption &character, PLAYER_TYPE type)
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

void ChooseNPCGameState::LoadCharacterSprites()
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

Rectangle ChooseNPCGameState::GetCurrentFrame(const CharacterOption &character) const
{
    const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(character.type);
    if (!sheet.frames.empty() && character.spriteAnimation.frameIndex < static_cast<int>(sheet.frames.size()))
    {
        return sheet.frames[character.spriteAnimation.frameIndex];
    }
    return Rectangle{0, 0, 64, 64};
}

void ChooseNPCGameState::init()
{
    // Detectar y establecer la ruta de assets
    DataFileManager::GetInstance().DetectAndSetAssetsPath();
    SpriteLoaderManager::GetInstance().DetectAndSetAssetsPath();

    // Cargar los personajes con sus datos y sprites
    LoadCharacterSprites();

    currentCharacterIndex = 0;
    arrowHovered[0] = false;
    arrowHovered[1] = false;
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

    // Detectar hover del ratón sobre los botones de flecha
    Vector2 mousePosition = GetMousePosition();
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Calcular posiciones para los botones
    Vector2 containerPos = GetContainerPos(screenWidth, screenHeight);
    float spriteWidth = CONTAINER_SIZE * CONTAINER_SCALE;
    float spriteHeight = spriteWidth;
    Vector2 spritePos = GetSpritePos(containerPos, spriteWidth, spriteHeight);

    // Resetear el array de hover
    arrowHovered[0] = false;
    arrowHovered[1] = false;

    // Verificar hover sobre cada botón
    for (int i = 0; i < 2; i++)
    {
        Rectangle btn = GetArrowButtonRect(i, spritePos, spriteWidth, screenHeight);
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

void ChooseNPCGameState::update(float deltaTime)
{
    if (!characters.empty() && currentCharacterIndex < static_cast<int>(characters.size()))
    {
        CharacterOption &character = characters[currentCharacterIndex];
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(character.type);

        character.spriteAnimation.timeAccumulator += deltaTime;
        if (character.spriteAnimation.timeAccumulator >= character.spriteAnimation.FRAME_DURATION)
        {
            character.spriteAnimation.timeAccumulator = 0.0f;
            character.spriteAnimation.frameIndex++;
            character.spriteAnimation.frameIndex %= sheet.spriteFrameCount;
        }
    }
}

void ChooseNPCGameState::DrawCenteredText(const char *text, float y, int fontSize, Color color) const
{
    int screenWidth = GetScreenWidth();
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    Vector2 textPos = {(screenWidth - textSize.x) / 2.0f, y};
    DrawTextEx(GetFontDefault(), text, textPos, (float)fontSize, 1.0f, color);
}

Vector2 ChooseNPCGameState::GetContainerPos(int screenWidth, int screenHeight) const
{
    return Vector2{(screenWidth - DISPLAY_SIZE) / 2.0f, (screenHeight - DISPLAY_SIZE) / 2.0f - 20.0f};
}

Vector2 ChooseNPCGameState::GetSpritePos(const Vector2 &containerPos, float spriteWidth, float spriteHeight) const
{
    return Vector2{containerPos.x + (DISPLAY_SIZE - spriteWidth) / 2.0f,
                   containerPos.y + (DISPLAY_SIZE - spriteHeight) / 2.0f};
}

Rectangle ChooseNPCGameState::GetArrowButtonRect(int index, const Vector2 &spritePos, float spriteWidth,
                                                 int screenHeight)
{
    // index 0 = flecha izquierda, index 1 = flecha derecha
    if (index == 0)
    {
        const char *leftArrow = "<";
        Vector2 leftArrowSize = MeasureTextEx(GetFontDefault(), leftArrow, ARROW_FONT_SIZE, 1);
        Vector2 leftArrowPos = {spritePos.x - ARROW_DISTANCE, (screenHeight - leftArrowSize.y) / 2.0f};

        return Rectangle{leftArrowPos.x - ARROW_PADDING, leftArrowPos.y - ARROW_PADDING,
                         leftArrowSize.x + ARROW_PADDING * 2, leftArrowSize.y + ARROW_PADDING * 2};
    }
    else
    {
        const char *rightArrow = ">";
        Vector2 rightArrowSize = MeasureTextEx(GetFontDefault(), rightArrow, ARROW_FONT_SIZE, 1);
        Vector2 rightArrowPos = {spritePos.x + spriteWidth + ARROW_DISTANCE - rightArrowSize.x,
                                 (screenHeight - rightArrowSize.y) / 2.0f};

        return Rectangle{rightArrowPos.x - ARROW_PADDING, rightArrowPos.y - ARROW_PADDING,
                         rightArrowSize.x + ARROW_PADDING * 2, rightArrowSize.y + ARROW_PADDING * 2};
    }
}

void ChooseNPCGameState::DrawNavigationArrows(const Vector2 &spritePos, float spriteWidth)
{
    int screenHeight = GetScreenHeight();
    Color fillColor = {60, 60, 60, 255};

    // Dibujar flecha izquierda
    Rectangle leftRect = GetArrowButtonRect(0, spritePos, spriteWidth, screenHeight);
    const char *leftArrow = "<";
    Vector2 leftArrowSize = MeasureTextEx(GetFontDefault(), leftArrow, ARROW_FONT_SIZE, 1);
    Vector2 leftArrowPos = {spritePos.x - ARROW_DISTANCE, (screenHeight - leftArrowSize.y) / 2.0f};

    // Usar el array para determinar el color según el hover
    Color leftArrowColor = arrowHovered[0] ? SELECTED_ARROW_COLOR : UNSELECTED_ARROW_COLOR;
    if (characters.size() <= 1)
    {
        leftArrowColor = UNSELECTED_ARROW_COLOR;
    }

    DrawRectangleRec(leftRect, fillColor);
    DrawRectangleLinesEx(leftRect, 1.0f, leftArrowColor);
    DrawTextEx(GetFontDefault(), leftArrow, leftArrowPos, (float)ARROW_FONT_SIZE, 1.0f, leftArrowColor);

    // Dibujar flecha derecha
    Rectangle rightRect = GetArrowButtonRect(1, spritePos, spriteWidth, screenHeight);
    const char *rightArrow = ">";
    Vector2 rightArrowSize = MeasureTextEx(GetFontDefault(), rightArrow, ARROW_FONT_SIZE, 1);
    Vector2 rightArrowPos = {spritePos.x + spriteWidth + ARROW_DISTANCE - rightArrowSize.x,
                             (screenHeight - rightArrowSize.y) / 2.0f};

    // Usar el array para determinar el color según el hover
    Color rightArrowColor = arrowHovered[1] ? SELECTED_ARROW_COLOR : UNSELECTED_ARROW_COLOR;
    if (characters.size() <= 1)
    {
        rightArrowColor = UNSELECTED_ARROW_COLOR;
    }

    DrawRectangleRec(rightRect, fillColor);
    DrawRectangleLinesEx(rightRect, 1.0f, rightArrowColor);
    DrawTextEx(GetFontDefault(), rightArrow, rightArrowPos, (float)ARROW_FONT_SIZE, 1.0f, rightArrowColor);
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
        const SpriteSheet &sheet = SpriteLoaderManager::GetInstance().GetSpriteSheet(currentChar.type);
        Rectangle frame = GetCurrentFrame(currentChar);

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
        DrawNavigationArrows(containerPos, DISPLAY_SIZE);

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

void ChooseNPCGameState::pause() {}
void ChooseNPCGameState::resume() {}
