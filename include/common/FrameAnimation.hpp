#pragma once

// Estructura para controlar una animación basada en frames
struct FrameAnimation
{
    int frame = 0;              // Frame actual
    float timer = 0.0f;         // Acumulador de tiempo
    float frameDuration = 0.075f; // Duración de cada frame (segundos)
    bool loop = true;           // Si la animación hace loop
    bool finished = false;      // Marcado cuando termina (si no loop)
};