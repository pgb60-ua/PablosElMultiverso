#pragma once

typedef struct
{
    // Si presiona tecla arriba
    int north;
    // Si presiona tecla derecha
    int east;
    // Si presiona tecla abajo
    int south;
    // Si presiona tecla izquierda
    int west;
    // Si presiona tecla para pausar el juego
    int pause;
} InputComponent;
