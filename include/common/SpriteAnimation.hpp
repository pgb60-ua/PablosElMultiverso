#pragma once
extern "C" {
    #include <raylib.h>
}
struct SpriteAnimation
{
    inline const static float FRAME_DURATION = 0.075f; // Duración de cada frame en segundos
    int frameIndex = 0;          // Índice del frame actual
    float timeAccumulator = 0.0f; // Acumulador de tiempo para controlar la velocidad de animación
    bool flipped = false;      // Indica si la animación está girada horizontalmente
    Color color = WHITE;      // Color de tinte para la animación
};