#pragma once

extern "C"
{
#include <raylib.h>
}

// Representa el CENTRO de la entidad
typedef struct
{
    float x;
    float y;
} PositionComponent;

typedef struct
{
    float movementSpeed;
} MovementSpeedComponent;

typedef struct
{
    Vector2 velocity;
} VelocityComponent;
