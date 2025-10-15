#pragma once
#include <vector>
extern "C"
{
#include <raylib.h>
}
// Estructura para almacenar un sprite-sheet con sus frames
struct SpriteSheet
{
    Texture2D texture;              // La textura completa del sprite-sheet
    std::vector<Rectangle> frames;  // Rectángulos de cada frame en el sprite-sheet
    int spriteFrameCount = 0;       // Número de frames en el sprite-sheet  
};