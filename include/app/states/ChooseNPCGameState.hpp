#pragma once
#include "GameState.hpp"
#include "Types.hpp"
#include <vector>
#include <string>

extern "C" {
    #include <raylib.h>
}

struct SpriteFrame {
    int x;
    int y;
    int width;
    int height;
};

struct CharacterOption {
    PLAYER_TYPE type;
    std::string name;
    std::string description;
    Texture2D spriteSheet;
    std::string spriteSheetPath;
    std::vector<SpriteFrame> frames;
    int currentFrame;
};

class ChooseNPCGameState : public GameState {

    protected:
        std::vector<CharacterOption> characters;
        int currentCharacterIndex = 0;
        
        const Color SELECTED_ARROW_COLOR = YELLOW;
        const Color UNSELECTED_ARROW_COLOR = GRAY;
        
        void LoadCharacterSprites();
        SpriteFrame GetCurrentFrame(const CharacterOption& character) const;
        
        // Funciones helper para simplificar el código
        std::string GetDefaultCharacterName(PLAYER_TYPE type) const;
        void DrawCenteredText(const char* text, float y, int fontSize, Color color) const;
        void DrawNavigationArrows(const Vector2& spritePos, float spriteWidth) const;
        void LoadCharacterData(CharacterOption& character, PLAYER_TYPE type);

    public:
        ChooseNPCGameState();
        ~ChooseNPCGameState();
        virtual void init() override;
        virtual void handleInput() override;
        virtual void update(float deltaTime) override;
        void render() override;

        virtual void pause() override;
        virtual void resume() override;

        PLAYER_TYPE getSelectedCharacterType() const { return characters[currentCharacterIndex].type; }
        std::string getSelectedCharacterName() const { 
            if (currentCharacterIndex >= 0 && currentCharacterIndex < characters.size()) {
                return characters[currentCharacterIndex].name;
            }
            return "";
        }
};