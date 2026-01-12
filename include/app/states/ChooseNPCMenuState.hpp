#pragma once
#include "GameState.hpp"
#include "SpriteAnimation.hpp"
#include "Types.hpp"
#include <vector>
#include <string>

extern "C" {
    #include <raylib.h>
}

struct CharacterOption {
    PLAYER_TYPE type;
    std::string name;
    std::string description;
    SpriteAnimation spriteAnimation;
};

class ChooseNPCMenuState : public GameState {

    private:
        std::vector<CharacterOption> characters;
        int currentCharacterIndex = 0;
        bool arrowHovered[2] = {false, false}; // Array para rastrear hover: [0] = izquierda, [1] = derecha
        
        const Color SELECTED_ARROW_COLOR = YELLOW;
        const Color UNSELECTED_ARROW_COLOR = GRAY;
        
        void LoadCharacterSprites();
        
        // Funciones helper para simplificar el código
        std::string GetDefaultCharacterName(PLAYER_TYPE type) const;
        void DrawCenteredText(const char* text, float y, int fontSize, Color color) const;
        void DrawNavigationArrows(const Vector2& containerPos) const;
        void LoadCharacterData(CharacterOption& character, PLAYER_TYPE type);
        
        // Funciones auxiliares para calcular posiciones y rectángulos
        Vector2 GetContainerPos(int screenWidth, int screenHeight) const;
        Vector2 GetSpritePos(const Vector2& containerPos, float spriteWidth, float spriteHeight) const;
        // Función auxiliar estática para calcular rectángulos de botones de flecha
        static Rectangle GetArrowButtonRect(int index, const Vector2& containerPos, int screenHeight);

    public:
        ChooseNPCMenuState();
        ~ChooseNPCMenuState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;

        PLAYER_TYPE getSelectedCharacterType() const { return characters[currentCharacterIndex].type; }
        std::string getSelectedCharacterName() const { 
            if (currentCharacterIndex >= 0 && currentCharacterIndex < static_cast<int>(characters.size())) {
                return characters[currentCharacterIndex].name;
            }
            return "";
        }

        // Constantes para los botones de navegación
        static constexpr float CONTAINER_SIZE = 64.0f;
        static constexpr float CONTAINER_SCALE = 6.0f;
        static constexpr float DISPLAY_SIZE = CONTAINER_SIZE * CONTAINER_SCALE;
        static constexpr int ARROW_FONT_SIZE = 90;
        static constexpr float ARROW_PADDING = 20.0f;
        static constexpr float ARROW_DISTANCE = 160.0f;
};